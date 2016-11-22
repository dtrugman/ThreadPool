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
