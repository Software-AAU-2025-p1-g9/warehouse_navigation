
#ifndef WAREHOUSE_NAVIGATION_WAREHOUSE_GENERATOR_H
#define WAREHOUSE_NAVIGATION_WAREHOUSE_GENERATOR_H
#include "warehouse.h"


// ===============================================================
// Maks antal edges per node (4 ortogonale + 4 diagonale)
// ===========
// ===============================================================
// Maks antal edges per node (4 ortogonale + 4 diagonale)
// ===============================================================
#define MAX_EDGES 8

// ===============================================================
// Forward declarations for edge og node
// ===============================================================
struct node;  // fortæller at struct node eksisterer
typedef struct node node;

struct edge;  // fortæller at struct edge eksisterer
typedef struct edge edge;

// ===============================================================
// Warehouse grid node
//  Repræsenterer et felt i lageret.
//  Indeholder kun koordinater og pointer til graf-node
// ===============================================================
typedef struct NodeGrid {
    int x, y;                       // position i lageret
    int id;                          // unikt nummer for denne node
    node* graphNode;                 // peger til graf-node (kun hvis walkable)
} NodeGrid;

// ===============================================================
// Funktioner til Warehouse
// ===============================================================
NodeGrid** createWarehouse(int width, int height);
void generateWarehouseLayout(NodeGrid** grid, int width, int height, node*** shelves, int* shelf_count, node*** dropoffs, int* dropoff_count, int corridorWidth, int shelfBlockWidth);
void setPickupPoints(NodeGrid** grid, int width, int height, node*** pickups, int* pickup_count);
void printWarehouse(NodeGrid** grid, int width, int height, node** shelves, int shelf_count, node** dropoffs, int dropoff_count, node** pickups, int pickup_count);
void freeWarehouse(NodeGrid** grid, int width, int height, node** shelves, node** dropoffs, node** pickups);

// ===============================================================
// Funktioner til grafen
// ===============================================================
void createGraphFromWarehouse(int width, int height, NodeGrid** grid, node** shelves, int shelf_count, node*** outList, int* outCount);
void freeGraph(node** list, int count);

#endif //WAREHOUSE_NAVIGATION_WAREHOUSE_GENERATOR_H
//====================================================



