#include "warehouse.h"
#include <stdlib.h>
#include <time.h>

int node_pos(int size_x, int x, int y){
    return y * size_x + x;
}

static node* get_random_node(node* nodes, int size_x, int size_y) {
    int total = size_x * size_y;
    int index = rand() % total;
    return &nodes[index];
}

void generate_simple_loop_route(worker* w, node* nodes, int size_x, int size_y) {
    w -> route_length = 0;
    w -> position = 0;

    node* A = get_random_node(nodes, size_x, size_y);
    node* B = get_random_node(nodes, size_x, size_y);
    node* C = get_random_node(nodes, size_x, size_y);

    w -> route[w -> route_length++] = A;
    w -> route[w -> route_length++] = B;
    w -> route[w -> route_length++] = C;
    w -> route[w -> route_length++] = A;             //goes back to start to make it a loop
}
