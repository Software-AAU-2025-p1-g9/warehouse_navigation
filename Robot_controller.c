//
// Created by matus on 03/12/2025.
//
#include <stdio.h>
#include "Robot_controller.h"
#include <warehouse.h>
#include "astar.h"
#include "algorithms.h"


void assign_robot_path(Robot* r, order o, node** warehouse, int map_id, int height, int width) {
    int current_x = r->goal_2->x;
    int current_y = r->goal_2->y;
    node* current_node = r->goal_2;

    r->goal_1=o.node_1;
    r->goal_2=o.node_2;

    reset_g(warehouse, width, height, node_pos(width, current_x, current_y));
    printf("reset_g virker\n"); //Debug
    astar(warehouse, current_x, current_y, r->goal_1->x, r->goal_1->y, map_id, height * width);
    printf("A* virker\n"); //Debug
    find_shortest_path(&r->path, &r->path_length, current_node, r->goal_1, map_id);
    printf("find_shortest_path virker?\n"); //Debug


    r->has_order = 1;
    r->path_pos = 0;
}

void move_robot(Robot* r, float* global_time) {
    *global_time = r->time_at_next_stop;
    r->path_pos++;
    if (r->path_pos == r->path_length) {
        if (r-> path [r -> path_length - 1]->dest == r-> goal_1 ) {
            printf("Robot har nået mål 1!\n");
            //Kør a star
            // Find shortest path

        }
        else{
            printf("Robot har nået mål 2!\n");
            r-> has_order = 0;

        }
        r->time_at_next_stop = *global_time + 10;
    }
    else {
        r->time_at_next_stop = *global_time + r-> path [r-> path_pos]->cost ;

    }




}

