#ifndef WORKER_H
#define WORKER_H

#include "warehouse.h"

#define NUM_STOPS 4

typedef struct {
    edge** route;
    float stay_time[NUM_STOPS];
    int route_length;
    int current_edge;
    node* stops[3];
} worker;

void generate_simple_loop_route(worker* w,
                                int size_y, int size_x,
                                node nodes[size_y][size_x]);

#endif
