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

#add_executable(TEST-ALL ${INCLUDE_SRC} ${TEST_SRC})
add_executable(TEST-ALL ${INCLUDE_SRC}
    test/main.cpp
    test/view_tag.cpp
    test/view_tag_base.cpp
    test/proj.cpp
    test/find_if.cpp
    test/push_pop_base.cpp
)
