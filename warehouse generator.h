
#ifndef WAREHOUSE_NAVIGATION_WAREHOUSE_GENERATOR_H
#define WAREHOUSE_NAVIGATION_WAREHOUSE_GENERATOR_H

// ===============================================================
// En Node er en enkelt plads i lageret.
// Den kan være:
//  - en almindelig gulvplads
//  - en hylde/shelf
//  - et dropoff-punkt
//  - et sted hvor der ligger en vare
// ===============================================================

// --------------------------------------------------------------
// Structs for graf (nodes, edges, orders)
// --------------------------------------------------------------

typedef struct edge edge;
typedef struct node node;
typedef struct order order;

struct node {
    const int x;               // x-position i lageret (kolonne) – konstant
    const int y;               // y-position i lageret (række) – konstant

    int neighbour_count;       // antal nabo-noder (typisk 8)
    int successor_count;       // antal successor-edges (kanter ud)
    int predecessor_count;     // antal predecessor-edges (kanter ind)

    edge** successors;         // array af pointere til alle kanter der går *ud* fra denne node
    edge** predecessors;       // array af pointere til alle kanter der går *ind* i denne node

    double* g;                 // bruges af pathfinding-algoritmer (ikke relevant for layout)
    double* h;                 // heuristisk værdi til pathfinding
    double* rhs;               // bruges af LPA*/D* algoritmer
};

struct edge {
    node* source;              // start-node for kanten
    node* dest;                // slut-node for kanten
    double cost;               // vægt (1 for lige bevægelser, sqrt(2) for diagonale)
};

struct order {
    node* node_1;              // første lokation (pickup eller første del af ordren)
    node* node_2;              // anden lokation (destination eller anden del af ordren)
};

// ===============================================================
// Warehouse grid node (ikke samme som graf-node!)
// ===============================================================
typedef struct NodeGrid {
    int x, y;                  // position i lageret
    int id;                    // unikt nummer for denne node

    int isShelf;               // 1 = hylde, 0 = tom plads
    int isDropoff;             // 1 = dropoff punkt, 0 ellers
    int isPickup;              // 1 = pickup punkt, 0 ellers
    int hasItem;               // 1 = vare placeret, 0 ellers

    node* graphNode;           // peger til graf-node (kun hvis walkable)
} NodeGrid;

// ===============================================================
// Warehouse struktur
// ===============================================================
typedef struct Warehouse {
    int width;                 // Hvor bredt er lageret (antal kolonner)
    int height;                // Hvor højt er lageret (antal rækker)

    NodeGrid** grid;           // 2D-array af NodeGrid

    node* pickup;              // punkt hvor robot henter varer
} Warehouse;

// ===============================================================
// Funktioner til Warehouse
// ===============================================================
Warehouse* createWarehouse(int width, int height);
void generateFishbone(Warehouse* w, int corridorWidth, int shelfBlockWidth);
void autoPlaceItems(Warehouse* w, int count);
void setPickupPoint(Warehouse* w);
void printWarehouse(Warehouse* w);
void freeWarehouse(Warehouse* w);

// ===============================================================
// Funktioner til grafen
// ===============================================================
void createGraphFromWarehouse(Warehouse* w, node*** outNodes, int* outCount);
void freeGraph(node** list, int count);
// ===============================================================
// Warehouse: hele lageret
// Består af:
//
//  - grid: 2D array (array af arrays) af Nodes
//  - shelves: array af pointers til de Nodes der er hylder
//  - dropoffs: array af pointers til dropoff-punkter
//
// ===============================================================

#endif //WAREHOUSE_NAVIGATION_WAREHOUSE_GENERATOR_H