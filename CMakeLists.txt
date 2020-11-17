cmake_minimum_required(VERSION 3.17)
project(channel)

set(CMAKE_CXX_STANDARD 14)

add_executable(channel main.cpp Channel.hpp)

target_link_libraries(channel pthread)