#ifndef WORKER_H
#define WORKER_H

#include "warehouse.h"

#define NUM_TIME 4

typedef struct {
    edge** route;
    float stay_time[NUM_TIME];
    int route_length;
    int position;
} worker;

void generate_simple_loop_route(worker* w,
                                int size_y, int size_x,
                                node nodes[size_y][size_x]);

// Find a direct edge from "from" -> "to"
edge* find_edge(node* from, node* to);

#endif
