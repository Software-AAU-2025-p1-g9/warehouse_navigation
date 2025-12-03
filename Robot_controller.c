//
// Created by matus on 03/12/2025.
//
#include <stdio.h>
#include "Robot_controller.h"
#include <string.h>
#include <warehouse.h>


void assign_robot_path(Robot* r, order o) {
    r->goal_1=o.node_1;
    r->goal_2=o.node_2;
    //A star
    //Find shortest path
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

