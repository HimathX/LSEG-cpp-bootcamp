cmake_minimum_required(VERSION 3.10)
project(FlowerExchange)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable -O2 optimization flag from day one
set(CMAKE_CXX_FLAGS "-O2 ${CMAKE_CXX_FLAGS}")

# Collect all source files
file(GLOB SOURCES "src/*.cpp")

add_executable(FlowerExchange ${SOURCES})
