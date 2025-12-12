
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



// Funktioner til Warehouse
// ===============================================================
node** createWarehouse(int width, int height);
void adjustWarehouseSize(int* width, int corridorWidth);
void generateWarehouseLayout(node** grid, int width, int height, node*** shelves, int* shelf_count,
                             node*** dropoffs, int* dropoff_count, node*** pickups, int* pickup_count,
                             int corridorWidth);
void printWarehouse(node** grid, int width, int height, node** shelves, int shelf_count, node** dropoffs, int dropoff_count, node** pickups, int pickup_count);
void freeWarehouse(node** grid, int width, int height, node** shelves, node** dropoffs, node** pickups);

// ===============================================================
// Funktioner til grafen
// ===============================================================
void create_graph(int width, int height, node*** grid, edge** edges, int* edge_count);
void freeGraph(node** list, int count);

#endif //WAREHOUSE_NAVIGATION_WAREHOUSE_GENERATOR_H
//====================================================



