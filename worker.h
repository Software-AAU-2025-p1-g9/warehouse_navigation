#ifndef WORKER_H
#define WORKER_H
#define NUM_STOPS 3
#define MAX_NODE_NEIGHBOUR 8
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

    node* backed_up_node;
    float backed_up_costs[MAX_NODE_NEIGHBOUR];
} worker;


void generate_simple_loop_route(worker* w,
                                int size_y, int size_x, node nodes[size_y][size_x],
                                map_data* map_datas, enum algorithm alg, float global_time);

void move_worker(worker* w, float* global_time);

#endif
