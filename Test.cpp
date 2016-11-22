#include <sstream>
#include <thread>
#include <functional>

#include "catch.hpp"

#include "ThreadPool.hpp"

using namespace std;

uint64_t getTid()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return std::stoull(ss.str());
}

TEST_CASE("Basic test", "[sanity]")
{
    uint64_t myTid = getTid();

    ThreadPool tp(2);

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
