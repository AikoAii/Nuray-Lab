cmake_minimum_required(VERSION 3.16)
project(level0_playground CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(Playgorund src/main.cpp)

target_link_libraries(Playgorund PRIVATE raylib)
