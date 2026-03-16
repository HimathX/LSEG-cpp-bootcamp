# use CMake version 3.10 or higher
cmake_minimum_required(VERSION 3.10)
# create a project called FlowerExchange
project(FlowerExchange)
# compile using C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable -O2 optimization flag from day one - Keep whatever flags the user has set, but add -O2 to the front of the list
set(CMAKE_CXX_FLAGS "-O2 ${CMAKE_CXX_FLAGS}")

# Collect all source files from src/ directory
file(GLOB SOURCES "src/*.cpp")

# build one executable named FlowerExchange from the collected source files
add_executable(FlowerExchange ${SOURCES})


