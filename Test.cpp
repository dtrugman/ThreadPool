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

#include <sstream>
#include <thread>
#include <functional>

#include "catch.hpp"

#include "ThreadPool.hpp"

using namespace std;

static const size_t SMALL_POOL_SIZE = 2;
static const size_t REGULAR_POOL_SIZE = 5;
static const size_t LARGE_POOL_SIZE = 20;
static const size_t XLARGE_POOL_SIZE = 100;

uint64_t getTid()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return std::stoull(ss.str());
}

TEST_CASE("Basic test", "[sanity]")
{
    uint64_t myTid = getTid();

    ThreadPool tp(SMALL_POOL_SIZE);

    SECTION("Different execution thread")
    {
        uint64_t execTid = tp.addTask(getTid).get();
        REQUIRE(myTid != execTid);
    }

    SECTION("Basic method execution")
    {
        const int RESULT = 10;

        int result = tp.addTask([RESULT]() { return RESULT; }).get();
        REQUIRE(result == RESULT);
    }
}

void testSmallTasksExecution(size_t workersCount, size_t tasksCount)
{
    uint8_t result[tasksCount] = { 0 };

    // This scope will allow the tasks to execute before
    // continuing to the actual REQUIRE clauses
    {
        ThreadPool tp(workersCount);

        for (size_t i = 0; i < tasksCount; i++)
        {
            tp.addTask([&result, i]() { result[i] = (uint8_t)i; });
        }
    }

    for (size_t i = 0; i < tasksCount; i++)
    {
        REQUIRE(result[i] == (uint8_t)i);
    }
}

TEST_CASE("Load test", "[load]")
{
    SECTION("Standard size pool")
    {
        SECTION("Execute 1,000 small tasks")
        {
            testSmallTasksExecution(REGULAR_POOL_SIZE, 1000);
        }

        SECTION("Execute 100,000 small tasks")
        {
            testSmallTasksExecution(XLARGE_POOL_SIZE, 100000);
        }
    }

    SECTION("Very large pool")
    {
        SECTION("Execute 1,000 small tasks")
        {
            testSmallTasksExecution(REGULAR_POOL_SIZE, 1000);
        }

        SECTION("Execute 100,000 small tasks")
        {
            testSmallTasksExecution(XLARGE_POOL_SIZE, 100000);
        }
    }
}

TEST_CASE("Graceful/Immediate stop tests", "[algorithm]")
{
    // Use smallets pool size in order for the workers not to finish the job
    // before the stop/d'tor is called

    size_t workersCount = SMALL_POOL_SIZE;
    size_t tasksCount = 10000;

    uint8_t result[tasksCount] = { 0 };

    bool expectAllSuccessful;

    SECTION("Graceful destruction")
    {
        // This scope will allow the tasks to execute before
        // continuing to the actual REQUIRE clauses
        {
            ThreadPool tp(workersCount);

            for (size_t i = 0; i < tasksCount; i++)
            {
                tp.addTask([&result, i]() { result[i] = (uint8_t)i; });
            }
        }

        expectAllSuccessful = true;
    }

    SECTION("Graceful stop")
    {
        ThreadPool tp(workersCount);

        for (size_t i = 0; i < tasksCount; i++)
        {
            tp.addTask([&result, i]() { result[i] = (uint8_t)i; });
        }

        tp.stop(false);

        expectAllSuccessful = true;
    }

    SECTION("Immediate stop")
    {
        ThreadPool tp(workersCount);

        for (size_t i = 0; i < tasksCount; i++)
        {
            tp.addTask([&result, i]() { result[i] = (uint8_t)i; });
        }

        tp.stop(true);

        expectAllSuccessful = false;
    }

    bool allSuccessful = true;

    for (size_t i = 0; i < tasksCount; i++)
    {
        allSuccessful &= (result[i] == (uint8_t)i);
    }

    REQUIRE(allSuccessful == expectAllSuccessful);
}
