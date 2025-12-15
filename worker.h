#ifndef WORKER_H
#define WORKER_H
#define NUM_STOPS 3
#define MAX_NODE_NEIGHBOUR 8   // passer til grid med op til 8 naboer (N, S, E, W + diagonaler)
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

    // backup af predecessor-edge costs for den node vi sidst ændrede
    node* backed_up_node;
    float backed_up_costs[MAX_NODE_NEIGHBOUR];
    int backed_up_count;
} worker;


void generate_simple_loop_route(worker* w,
                                int size_y, int size_x, node nodes[size_y][size_x],
                                map_data* map_datas, enum algorithm alg, float global_time);

void move_worker(worker* w, float* global_time);

#endif
