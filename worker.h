#ifndef WORKER_H
#define WORKER_H
#define NUM_STOPS 3
#include "algorithms.h"
#include "warehouse.h"


typedef struct {
    edge** route;
    int route_length;
    int current_edge;

    float time_stop;
    node* current_node;

    float stay_time[NUM_STOPS];
    node* stops[NUM_STOPS];
} worker;

void generate_simple_loop_route(worker* w,
                                int size_y, int size_x, node nodes[size_y][size_x],
                                map_data* map_datas, enum algorithm alg, float global_time);

void move_worker(worker* w, float* global_time,
                int size_x, enum algorithm alg);

#endif
