//
// Created by matus on 03/12/2025.
//
#include <stdio.h>
#include "Robot_controller.h"
#include <string.h>
#include <warehouse.h>


typedef struct {
    node* current_edge;
    node* next_node;
    float timeatnextstop;
    node* goal_1;
    node* goal_2;
}Robot;


void update_robot(Robot* r, double* global_time) {
    *global_time = r->timeatnextstop;
    r->current_edge = r->next_node;
    if (r->current_edge == r->goal_1) {
        printf("Robot har nået mål 1!\n");
    } else if (r->current_edge == r->goal_2) {
        printf("Robot har nået mål 2!\n");
    }

    r->timeatnextstop = *global_time + 1.0;
}
int main(){
    node n1 = {1};
    node n2 = {2};

    Robot r = {&n1, &n2, 0.0, &n1, &n2};

    double global_time = 0.0;

    for(int i = 0; i < 4; i++) {
        update_robot(&r, &global_time);
        printf("Robot er nu på node %d, global tid = %.1f\n", global_time);
    }

    return 0;
}
