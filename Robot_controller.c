//
// Created by matus on 03/12/2025.
//
#include <stdio.h>
#include "Robot_controller.h"

#include <stdlib.h>
#include <warehouse.h>
#include "astar.h"

void assign_robot_path(robot* r, float* global_time, node** warehouse, int height, int width, node* goal_node, map_data* map_datas, enum algorithm algorithm) {
    *global_time = r->time_at_next_stop;
    switch (algorithm) {
        case D_STAR_LITE:
            assign_robot_path_d_star_lite(r, *global_time, warehouse, height, width, goal_node, map_datas);
            break;
        case LPA_STAR:
            assign_robot_path_lpa_star(r, *global_time, warehouse, height, width, goal_node, map_datas);
            break;
        default:
            assign_robot_path_a_star(r, *global_time, warehouse, height, width, goal_node);
    }
    if (r->path_copy != NULL) {
        free(r->path_copy);
    }
    r->path_copy = malloc(sizeof(edge) * r->path_length);
    for (int i = 0; i < r->path_length; i++) {
        r->path_copy[i] = *r->path[i];
    }
}

void assign_robot_path_a_star(robot* r, float global_time, node** warehouse, int height, int width, node* goal_node) {
    int map_id = node_pos(width, r->current_node->x, r->current_node->y);

    reset_g(warehouse, width, height, map_id);
    astar(warehouse, r->current_node->x, r->current_node->y, goal_node->x, goal_node->y, map_id, height * width);
    find_shortest_path(&r->path, &r->path_length, r->current_node, goal_node, map_id);

    r->path_pos = 0;
    float move_time = r->path_length > 0 ? r->path[0]->cost : 10;
    r->time_at_next_stop = global_time + move_time;
}

void assign_robot_order(robot* r, order o) {
    r->goal_1=o.node_1;
    r->goal_2=o.node_2;
    r->has_order = 1;
}

void assign_robot_path_lpa_star(robot* r, float global_time, node** warehouse, int height, int width, node* goal_node, map_data* map_datas) {
    int map_id = node_pos(width, r->current_node->x, r->current_node->y);
    if (map_datas[map_id].last_variable_node == NULL) {
        initialize_lpa_star(warehouse, width, height, r->current_node, goal_node, map_datas);
    }
    //lpa_star(r->current_node, goal_node, map_datas, map_id, warehouse, width, height);
    lpa_star(r->current_node, goal_node, map_datas, map_id);

    if (r->path != NULL) {
        free(r->path);
    }
    find_shortest_path(&r->path, &r->path_length, r->current_node, goal_node, map_id);
    r->path_pos = 0;
    float move_time = r->path_length > 0 ? r->path[0]->cost : 10;
    r->time_at_next_stop = global_time + move_time;
}

void assign_robot_path_d_star_lite(robot* r, float global_time, node** warehouse, int height, int width, node* goal_node, map_data* map_datas) {
    int map_id = node_pos(width, goal_node->x, goal_node->y);
    if (map_datas[map_id].last_variable_node == NULL) {
        initialize_d_star_lite(warehouse, width, height, r->current_node, goal_node, map_datas);
    }
    //d_star_lite(r->current_node, goal_node, map_datas, map_id, warehouse, width, height);
    d_star_lite(r->current_node, goal_node, map_datas, map_id);

    if (r->path != NULL) {
        free(r->path);
    }
    find_shortest_path_d_star_lite(&r->path, &r->path_length, r->current_node, goal_node, map_id);
    r->path_pos = 0;
    float move_time = r->path_length > 0 ? r->path[0]->cost : 10;
    r->time_at_next_stop = global_time + move_time;
}

void move_robot(robot* r, float* global_time) {
    *global_time = r->time_at_next_stop;
    r->path_pos++;
    if (r->path_pos == r->path_length) {
        r->current_node = r->path[r->path_pos - 1]->dest;
        r->time_at_next_stop = *global_time + 10;
    }
    else {
        r->current_node = r->path[r->path_pos]->source;
        r->time_at_next_stop = *global_time + r->path[r->path_pos]->cost;
    }
}