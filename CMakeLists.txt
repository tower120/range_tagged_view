cmake_minimum_required(VERSION 3.10)
project(range_tagged_view)

set(CMAKE_CXX_STANDARD 17)


include_directories(
    include
    third_party/doctest
    third_party/range-v3/include
)

file(GLOB_RECURSE INCLUDE_SRC include/*.hpp)
file(GLOB TEST_SRC test/*.cpp)
#file(GLOB TEST_SRC test/pure_main.cpp)

#enable_testing()

add_executable(TEST-ALL ${INCLUDE_SRC} ${TEST_SRC})

add_executable(console ${INCLUDE_SRC} console/main.cpp)