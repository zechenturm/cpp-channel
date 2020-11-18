cmake_minimum_required(VERSION 3.17)
project(channel)

set(CMAKE_CXX_STANDARD 14)

add_executable(channel main.cpp include/Channel.hpp include/RingBuffer.hpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

include_directories(include)

add_executable(tests test/test.cpp test/test_ringbuffer.cpp test/test.cpp test/test_ringbuffer.cpp)
add_test(NAME tests COMMAND tests)
enable_testing()

target_link_libraries(tests gtest pthread)
target_link_libraries(channel pthread)