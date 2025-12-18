#ifndef WORKER_H
#define WORKER_H
#define NUM_STOPS 3
#define MAX_NODE_NEIGHBOURS 8

#include "warehouse.h"
//#include "algorithms_print.h"
#include "algorithms.h"
#include "astar.h"

typedef struct {
    edge** route;
    int route_length;
    int current_edge;
    float time_at_next_stop;
    node* current_node;

    float stay_time[NUM_STOPS];
    node* stops[NUM_STOPS];

    node* backed_up_node;
    float backed_up_costs[MAX_NODE_NEIGHBOURS];
} worker;


void generate_worker_route(worker* w, int size_y, int size_x, node** nodes, map_data* map_datas, enum algorithm alg);

void move_worker(worker* w, float* global_time);

#endif
