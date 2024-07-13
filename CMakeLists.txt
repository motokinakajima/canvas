cmake_minimum_required(VERSION 3.26)
project(canvas)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(canvas
        canvas.h
        main.cpp)
