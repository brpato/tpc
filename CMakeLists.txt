cmake_minimum_required(VERSION 4.3)
project(tpc)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

add_executable(tpc main.cpp)

target_link_libraries(tpc raylib)