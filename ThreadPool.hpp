/*
    Copyright 2016 Daniel Trugman

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <iostream>
#include <deque>
#include <mutex>
#include <vector>
#include <thread>
#include <future>
#include <stdexcept>
#include <functional>
#include <condition_variable>

// ----------------------------------------------------------------------------
// Thread pool module decleration
// ----------------------------------------------------------------------------

class ThreadPool
{
public:
    ThreadPool(size_t size);
    virtual ~ThreadPool();

    template < class Func, class... Args >
    auto addTask(Func&& func, Args&&... args)
        -> std::future<typename std::result_of<Func(Args...)>::type>;

private:
    typedef std::function<void()> Task;
    typedef std::deque<Task>      TasksPool;
    typedef std::thread           Worker;
    typedef std::vector<Worker>   WorkersPool;

    struct Shared
    {
        Shared() : run(true) {}

        bool                    run;
        TasksPool               tasks;
        std::mutex              mutex;
        std::condition_variable cond;
    };

private:
    static void worker(size_t id, Shared & shared);

private:
    WorkersPool _workers;
    Shared      _shared;
};

// ----------------------------------------------------------------------------
// Thread pool module implementation
// ----------------------------------------------------------------------------

ThreadPool::ThreadPool(size_t size)
{
    for (size_t id = 0; id < size; id++)
    {
        _workers.emplace_back(worker, id, std::ref(_shared));
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> guard(_shared.mutex);

        _shared.run = false;
        _shared.cond.notify_all();
    }

    for (Worker & w : _workers)
    {
        w.join();
    }
}

template < class Func, class... Args >
auto ThreadPool::addTask(Func&& func, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type>
{
    using result_type = typename std::result_of<Func(Args...)>::type;

    std::lock_guard<std::mutex> guard(_shared.mutex);

    if (!_shared.run)
    {
        throw std::runtime_error("Can't add tasks when not running");
    }

    auto task = std::make_shared<std::packaged_task<result_type()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
    _shared.tasks.emplace_back([task]() { (*task)(); });
    auto result = task->get_future();

    if (_shared.tasks.size() == 1)
    {
        _shared.cond.notify_one();
    }

    return result;
}

void ThreadPool::worker(size_t id, Shared & shared)
{
    // Use a unique lock as we're going to wait on a cond using it
    std::unique_lock<std::mutex> lock(shared.mutex);

    while (true)
    {
        if (shared.tasks.empty())
        {
            // Stop if requested
            if (!shared.run)
            {
                lock.unlock();
                break;
            }

            while (shared.run && shared.tasks.empty())
            {
                shared.cond.wait(lock);
            }

            // Because many things could've happened while sleeping
            // after waking up we restart the entire cycle, this way
            // we can detect stop requests and make sure there are still
            // tasks in the queue
            continue;
        }

        // Get the first task
        Task task = std::move(shared.tasks.front());
        shared.tasks.pop_front();

        // Do actual work while lock not owned
        lock.unlock();
        task();
        lock.lock();
    }
}

#endif // THREAD_POOL_HPP
