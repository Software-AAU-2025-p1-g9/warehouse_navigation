struct edge;
struct node;
struct order;
typedef struct edge edge;
typedef struct node node;
typedef struct order order;



struct node {
    const int x;
    const int y;
    int neighbour_count;
    edge** successors; //array af pointere
    edge** predecessors; //array af pointere
    double* g;
    double* h;
    double* rhs;
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
