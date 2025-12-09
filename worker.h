#ifndef WORKER_H
#define WORKER_H
#define NUM_TIME 4


struct edge;
struct node;
struct order;

typedef struct edge edge;
typedef struct node node;
typedef struct order order;
typedef struct {
    edge** route;
    float stay_time[NUM_TIME];
    int route_length;
    int position;
} worker;

struct node {
    int x;
    int y;
    int neighbour_count;
    edge** successors; //array af pointere
    edge** predecessors; //array af pointere
    int* g;
    int* h;
    int* rhs;
};

struct edge {
    node* source;
    double cost;
    node* dest;
};

struct order {
    node node_1;
    node node_2;

};
// Converts (x, y) grid coordinates into a single integer
int node_pos(int size_x, int x, int y);

/* This function generate a loop route
 * and create a test route for these "workers" */
void generate_simple_loop_route(worker* w, int size_y, int size_x,
                                node nodes[size_y][size_x]);

// Find edges that goes from "from" -> "to"
edge* find_edge(node* from, node* to);

void find_shortest_path(edge*** path, int* path_length, node* start_node, node* goal_node, int map_id);

#endif