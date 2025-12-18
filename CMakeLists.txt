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
        order_generator.c
        order_generator.h
        worker.c
        worker.h
)

add_executable(warehouse_navigation_print
        main.c
        warehouse.c
        warehouse.h
        warehouseGenerator.c
        warehouseGenerator.h
        Robot_controller.c
        Robot_controller.h
        astar.h
        astar.c
        algorithms_print.c
        algorithms_print.h
        order_generator.c
        order_generator.h
        worker.c
        worker.h
)