cmake_minimum_required(VERSION 4.0)
project(warehouse_navigation C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(a_star_test
        astar.c
        main.c
        warehouse.c
        warehouse.h
        astar.h
)

add_executable(warehouse_navigation
        main.c
        warehouse.c
        warehouse.h
        warehouseGenerator.c
        warehouseGenerator.h
        Robot_controller.c
        Robot_controller.h
        astar.h
        astar.c
        algorithms.h
        algorithms.c
)

add_executable(algorithm_test
        algorithms.c
        warehouse.c
        algorithms.h)
add_executable(robot_controller Robot_controller.h Robot_controller.c)

add_executable(worker_test
        worker_simulation.c
        worker.c
        worker.h
        algorithms.h
        algorithms.c
        astar.h
        astar.c
        warehouse.c
        warehouse.h
)