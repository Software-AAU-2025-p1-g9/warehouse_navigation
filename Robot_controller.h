//
// Created by matus on 03/12/2025.
//

#include "warehouse.h"
#include "algorithms_print.h"
//#include "algorithms.h"


typedef struct {
    int path_pos;
    int path_length;
    edge** path;
    edge* path_copy;
    float time_at_next_stop;
    node* goal_1;
    node* goal_2;
    int has_order;
    node* current_node;
    int idle;
} robot;

void move_robot(robot* r, float* global_time);
void assign_robot_order(robot* r, order o);
void assign_robot_path(robot* r, float* global_time, node** warehouse, int height, int width,
                       node* goal_node, map_data* map_datas, enum algorithm algorithm, edge* edges, int edge_count);
void assign_robot_path_a_star(robot* r, float global_time, node** warehouse, int height, int width, node* goal_node);
void assign_robot_path_lpa_star(robot* r, float global_time, node** warehouse, int height, int width,
                                node* goal_node, map_data* map_datas, edge* edges, int edge_count);
void assign_robot_path_d_star_lite(robot* r, float global_time, node** warehouse, int height, int width,
                                   node* goal_node, map_data* map_datas, edge* edges, int edge_count);
