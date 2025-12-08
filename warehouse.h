struct edge;
struct node;
typedef struct edge edge;
typedef struct node node;

struct node {
    const int x;
    const int y;
    int neighbour_count;
    edge** successors; //array af pointere
    edge** predecessors; //array af pointere
    float* g;
    float* h;
    float* rhs;
};

struct edge {
    node* source;
    float cost;
    node* dest;
};


int node_pos(int size_x, int x, int y);
