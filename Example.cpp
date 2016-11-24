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
