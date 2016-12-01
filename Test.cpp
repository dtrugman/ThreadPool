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
#include <cstring>

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

void addFillArrayTasks(ThreadPool & tp, uint8_t * arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        tp.addTask([arr, i]() { arr[i] = (uint8_t)i; });
    }
}

bool arrayFull(uint8_t * arr, size_t size)
{
    bool retval = true;

    for (size_t i = 0; retval && i < size; i++)
    {
        retval &= (arr[i] == (uint8_t)i);
    }

    return retval;
}

void fillArrayTest(size_t workersCount, size_t tasksCount)
{
    uint8_t * arr = new uint8_t[tasksCount];
    REQUIRE(arr != NULL);

    // This scope will allow the tasks to execute before
    // continuing to the actual REQUIRE clauses
    {
        ThreadPool tp(workersCount);

        addFillArrayTasks(tp, arr, sizeof(arr));
    }

    bool full = arrayFull(arr, sizeof(arr));
 
    delete [] arr;

    REQUIRE(full);
}

TEST_CASE("Load test", "[load]")
{
    SECTION("Standard size pool")
    {
        SECTION("Execute 1,000 small tasks")
        {
            fillArrayTest(REGULAR_POOL_SIZE, 1000);
        }

        SECTION("Execute 100,000 small tasks")
        {
            fillArrayTest(XLARGE_POOL_SIZE, 100000);
        }
    }

    SECTION("Very large pool")
    {
        SECTION("Execute 1,000 small tasks")
        {
            fillArrayTest(REGULAR_POOL_SIZE, 1000);
        }

        SECTION("Execute 100,000 small tasks")
        {
            fillArrayTest(XLARGE_POOL_SIZE, 100000);
        }
    }
}

TEST_CASE("Graceful/Immediate stop tests", "[algorithm]")
{
    // Use smallets pool size in order for the workers not to finish the job
    // before the stop/d'tor is called

    uint8_t arr[10000];

    size_t workersCount = SMALL_POOL_SIZE;

    SECTION("Graceful destruction")
    {
        // This scope will allow the tasks to execute before
        // continuing to the actual REQUIRE clauses
        {
            ThreadPool tp(workersCount);

            addFillArrayTasks(tp, arr, sizeof(arr));
        }

        REQUIRE(arrayFull(arr, sizeof(arr)));
    }

    SECTION("Graceful stop")
    {
        ThreadPool tp(workersCount);

        addFillArrayTasks(tp, arr, sizeof(arr));

        tp.stop(false);

        REQUIRE(arrayFull(arr, sizeof(arr)));
    }

    SECTION("Immediate stop")
    {
        ThreadPool tp(workersCount);

        addFillArrayTasks(tp, arr, sizeof(arr));

        tp.stop(true);

        REQUIRE(!arrayFull(arr, sizeof(arr)));
    }
}
