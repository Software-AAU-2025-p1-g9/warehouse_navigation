//
// Created by matus on 03/12/2025.
//
#include <stdio.h>
#include "Robot_controller.h"

#include <stdlib.h>
#include <warehouse.h>
#include "astar.h"
#include "algorithms.h"


void assign_robot_path(Robot* r, float global_time, node** warehouse, int map_id, int height, int width, int goal_x, int goal_y) {
    int current_x = 0;
    int current_y = 0;

    reset_g(warehouse, width, height, map_id);
    astar(warehouse, r->current_node->x, r->current_node->y, goal_x, goal_y, map_id, (height*width));
    find_shortest_path(&r->path, &r->path_length, r->current_node, &warehouse[goal_y][goal_x], map_id);

    r->path_pos = 0;
    float move_time = r->path_length > 0 ? r->path[0]->cost : 0;
    r->time_at_next_stop = global_time + move_time;
}

void assign_robot_order(Robot* r, order o) {
    r->goal_1=o.node_1;
    r->goal_2=o.node_2;
    r->has_order = 1;
}

void assign_robot_path_lpa_star(Robot* r, float global_time, node** warehouse, int size_x, int size_y, node* goal_node, map_data* map_datas) {
    int map_id = node_pos(size_x, r->current_node->x, r->current_node->y);
    if (map_datas[map_id].last_variable_node == NULL) {
        initialize_lpa_star(warehouse, size_x, size_y, r->current_node, goal_node, map_datas);
    }

    lpa_star(r->current_node, goal_node, map_datas, map_id);
    if (r->path != NULL) {
        free(r->path);
    }
    find_shortest_path(&r->path, &r->path_length, r->current_node, goal_node, map_id);
    r->path_pos = 0;
    float move_time = r->path_length > 0 ? r->path[0]->cost : 0;
    r->time_at_next_stop = global_time + move_time;
}

void assign_robot_path_d_star_lite(Robot* r, float global_time, node** warehouse, int size_x, int size_y, node* goal_node, map_data* map_datas) {
    int map_id = node_pos(size_x, goal_node->x, goal_node->y);
    if (map_datas[map_id].last_variable_node == NULL) {
        initialize_d_star_lite(warehouse, size_x, size_y, r->current_node, goal_node, map_datas);
    }

    d_star_lite(r->current_node, goal_node, map_datas, map_id);
    if (r->path != NULL) {
        free(r->path);
    }
    find_shortest_path_d_star_lite(&r->path, &r->path_length, r->current_node, goal_node, map_id);
    r->path_pos = 0;
    float move_time = r->path_length > 0 ? r->path[0]->cost : 0;
    r->time_at_next_stop = global_time + move_time;
}

void move_robot(Robot* r, float* global_time) {
    *global_time = r->time_at_next_stop;
    r->path_pos++;
    if (r->path_pos == r->path_length) {
        r->current_node = r->path[r->path_pos - 1]->dest;
        if (r->current_node == r->goal_1) {
            printf("Robot har nået mål 1 til tiden %04.1f!\n", *global_time);
        }
        else{
            printf("Robot har nået mål 2 til tiden %04.1f!\n", *global_time);
            r-> has_order = 0;

        }
        r->time_at_next_stop = *global_time + 10;
    }
    else {
        r->current_node = r->path[r->path_pos]->source;
        r->time_at_next_stop = *global_time + r->path[r->path_pos]->cost;
    }
}