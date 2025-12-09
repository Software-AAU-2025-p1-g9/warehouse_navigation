//
// Created by matus on 03/12/2025.
//

#include <warehouse.h>

#include "algorithms.h"


typedef struct {
    int path_pos;
    int path_length;
    edge** path;
    float time_at_next_stop;
    node* goal_1;
    node* goal_2;
    int has_order;
    node* current_node;
    int idle;
}Robot;

void move_robot(Robot* r, float* global_time);
void assign_robot_order(Robot* r, order o);
void assign_robot_path_lpa(Robot* r, node** warehouse, int size_x, int size_y, node* goal_node, map_data* map_datas);
void assign_robot_path(Robot* r, order o, node** warehouse, int map_id, int height, int width);
