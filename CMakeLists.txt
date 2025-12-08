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
        main.c
        warehouse.c)

add_executable(algorithm_test
        algorithms.c
        warehouse.c
        algorithms.h)
add_executable(robot_controller Robot_controller.h Robot_controller.c)
