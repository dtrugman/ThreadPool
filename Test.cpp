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

void testSmallTasksExecution(ThreadPool & tp, size_t tasksCount)
{
    uint8_t result[tasksCount] = { 0 };

    for (size_t i = 0; i < tasksCount; i++)
    {
        tp.addTask([&result, i]() { result[i] = (uint8_t)i; }).get();
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
        ThreadPool tp(REGULAR_POOL_SIZE);

        SECTION("Execute 1,000 small tasks")
        {
            testSmallTasksExecution(tp, 1000);
        }

        SECTION("Execute 100,000 small tasks")
        {
            testSmallTasksExecution(tp, 100000);
        }
    }

    SECTION("Very large pool")
    {
        ThreadPool tp(XLARGE_POOL_SIZE);

        SECTION("Execute 1,000 small tasks")
        {
            testSmallTasksExecution(tp, 1000);
        }

        SECTION("Execute 100,000 small tasks")
        {
            testSmallTasksExecution(tp, 100000);
        }
    }
}
