//
// Created by matus on 03/12/2025.
//

#include <warehouse.h>


typedef struct {
    int path_pos;
    int path_length;
    edge** path;
    float time_at_next_stop;
    node* goal_1;
    node* goal_2;
    int has_order;
}Robot;

void move_robot(Robot* r, float* global_time);
void assign_robot_path(Robot* r, order o);
