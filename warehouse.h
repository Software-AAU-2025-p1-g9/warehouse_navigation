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
    float* g;
    float* h;
    float* rhs;
};

struct edge {
    node* source;
    float cost;
    node* dest;
};

struct order {
 node node_1;
 node node_2;

};

int node_pos(int size_x, int x, int y);

int min(int n_1, int n_2);
int max(int n_1, int n_2);
float f_min(float n_1, float n_2);