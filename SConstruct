# Copyright 2016 Daniel Trugman
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# -----------------------------------------------------------------------------
# Set environment
# -----------------------------------------------------------------------------
env=Environment(CPPPATH = '',
                CPPDEFINES = [],
                LIBS = [ 'pthread' ],
                CXXFLAGS = "-std=c++11")

# -----------------------------------------------------------------------------
# Build flags
# -----------------------------------------------------------------------------
# Debug flag
debug = ARGUMENTS.get('debug', 0) # default is 0 = off
if int(debug):
   env.Append(CXXFLAGS = ' -g')

# -----------------------------------------------------------------------------
# Build example
# -----------------------------------------------------------------------------
example_files = [ 'Example.cpp' ]
example_app = 'Example.out'

env.Program(target = example_app,
            source = example_files)
# -----------------------------------------------------------------------------
# Build tests
# -----------------------------------------------------------------------------
test_files = [ 'TestMain.cpp', 'Test.cpp' ]
test_app = 'Test.out'

env.Program(target = test_app,
            source = test_files)
