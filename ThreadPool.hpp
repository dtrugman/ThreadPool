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

#include <deque>
#include <vector>
#include <thread>
#include <future>
#include <functional>

// ----------------------------------------------------------------------------
// Thread pool module decleration
// ----------------------------------------------------------------------------

class ThreadPool
{
public:
    ThreadPool(size_t size);
    virtual ~ThreadPool();

    template < class Func, class... Args >
    auto addTask(Func&& func, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type>;

private:
    typedef std::function<void()> Task;
    typedef std::deque<Task>      TasksPool;
    typedef std::thread           Worker;
    typedef std::vector<Worker>   WorkersPool;

private:
    TasksPool   _tasks;
    WorkersPool _workers;
};

// ----------------------------------------------------------------------------
// Thread pool module implementation
// ----------------------------------------------------------------------------

ThreadPool::ThreadPool(size_t size)
{
    // TODO: Implement
}

ThreadPool::~ThreadPool()
{
    // TODO: Implement
}

template < class Func, class... Args >
auto ThreadPool::addTask(Func&& func, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type>
{
    // TODO: Implement
    // NOTE: Running as deferred to fail red-to-green tests (execute on a different thread id)
    return std::async(std::launch::deferred, std::forward<Func>(func), std::forward<Args>(args)...);
}

#endif // THREAD_POOL_HPP
