#include <iostream>

#include "ThreadPool.hpp"

using namespace std;

int main()
{
    ThreadPool tp(5);

    auto f1 = tp.addTask([]()             { return 42; }); // No params
    auto f2 = tp.addTask([](int x)        { return x * x; }, 7); // Single param
    auto f3 = tp.addTask([](int x, int y) { return x * y; }, 7, 6); // Two params

    cout << f1.get() << endl;
    cout << f2.get() << endl;
    cout << f3.get() << endl;

    return 0;
}
