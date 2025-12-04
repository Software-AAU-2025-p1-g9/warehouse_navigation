cmake_minimum_required(VERSION 4.0)
project(warehouse_navigation C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(warehouse_navigation
        astar.c
        main.c
        warehouse.c
        warehouse.h
        astar.h)
