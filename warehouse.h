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
 node* node_1; //første order der skal fulføres
 node* node_2; //den anden order der skal fulføres

};

int node_pos(int size_x, int x, int y);

void OrderRandomizer(int Order_Amount, order Order_Array[], node* Pickup[], int Pick_Length, node* Dropoff[], int Drop_Length, node* Shelves[], int Shelve_Length);