# C++ ThreadPool

This project is a simple yet powerful and generic implementation of a thread pool in cpp11.

Linux: 
![TravisCI](https://travis-ci.org/dtrugman/ThreadPool.svg?branch=master)

Windows:
![AppVeyor](https://ci.appveyor.com/api/projects/status/yr5yagayu006a8eq/branch/master?svg=true)

## Code Example

The following snippet is the simplest usage possible:

```cpp
ThreadPool tp(5);

auto f1 = tp.addTask([]()      { return 42; });
auto f2 = tp.addTask([](int x) { return x * x; }, 7);

cout << f1.get() << endl;
cout << f2.get() << endl;
```

By taking full advantage of cpp11, this design is very generic:

1. Tasks can have any method prototype
2. We can use the return values from the tasks using future wrappers

## Installation

Just add ThreadPool.hpp to your project and compile using c++11 or newer.

## Compilation

Just run 'scons'

## Tests

Just run 'scons test'

## License

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
