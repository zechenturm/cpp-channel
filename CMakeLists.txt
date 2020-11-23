cmake_minimum_required(VERSION 3.17)
project(channel)

set(CMAKE_CXX_STANDARD 14)

add_library(channel INTERFACE)
target_include_directories(channel INTERFACE include)

add_executable(channel-example examples/main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(tests test/test.cpp test/test_ringbuffer.cpp test/test.cpp test/test_ringbuffer.cpp)
add_test(NAME tests COMMAND tests)
enable_testing()

target_link_libraries(tests channel gtest pthread)
target_link_libraries(channel-example channel pthread)

install(TARGETS channel DESTINATION include)
