#ifndef WAREHOUSE_H
#define WAREHOUSE_H
#define MAX_ROUTE 1000

struct edge;
struct node;
struct order;
typedef struct edge edge;
typedef struct node node;
typedef struct order order;


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

int node_pos(int size_x, int x, int y);

typedef struct {
    node* route[MAX_ROUTE];
    int route_length;
    int position;
} worker;

void generate_simple_loop_route(worker* w, node* nodes, int size_x, int size_y);

#endif
