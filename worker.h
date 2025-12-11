#ifndef WORKER_H
#define WORKER_H
#include "algorithms.h"
#include "warehouse.h"

#define NUM_STOPS 3

typedef struct {
    edge** route;
    float stay_time[NUM_STOPS];
    int route_length;
    int current_edge;
    node* stops[NUM_STOPS];
} worker;

void generate_simple_loop_route(worker* w,
                                int size_y, int size_x,
                                node nodes[size_y][size_x],
                                map_data* map_datas,
                                enum algorithm alg);

#endif
