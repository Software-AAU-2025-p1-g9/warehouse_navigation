#ifndef WAREHOUSE_H
#define WAREHOUSE_H

struct edge;
struct node;
struct order;
typedef struct edge edge;
typedef struct node node;
typedef struct order order;


#define MAX_EDGES 8 // for alle shelves uendelig cost til at starte med. kør efter 1,41/sqr(2). skal være i et array.
struct node {
     int x;
     int y;
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

int min(int n_1, int n_2);
int max(int n_1, int n_2);
float f_min(float n_1, float n_2);
int f_eq(float n_1, float n_2);
void OrderRandomizer(int Order_Amount, order Order_Array[], node* Pickup[], int Pick_Length, node* Dropoff[], int Drop_Length, node* Shelves[], int Shelve_Length);

#endif
