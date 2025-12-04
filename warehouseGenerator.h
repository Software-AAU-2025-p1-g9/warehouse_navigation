
#ifndef WAREHOUSE_NAVIGATION_WAREHOUSE_GENERATOR_H
#define WAREHOUSE_NAVIGATION_WAREHOUSE_GENERATOR_H
#include "warehouse.h"
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
// er included af warehouse.h

// ===============================================================
   // Warehouse grid node
    //  Repræsenterer et felt i lageret.
    //  Indeholder kun koordinater og en pointer til graf-node.
    //  Alle oplysninger om hylder, pickups og dropoffs håndteres i separate arrays.
// ===============================================================
typedef struct NodeGrid {
    int x, y;                  // position i lageret
    int id;                    // unikt nummer for denne node

    node* graphNode;           // peger til graf-node (kun hvis walkable)
} NodeGrid;

// ===============================================================
// Warehouse struktur
// ===============================================================
typedef struct Warehouse {
    int width;                 // Hvor bredt er lageret (antal kolonner)
    int height;                // Hvor højt er lageret (antal rækker)
    NodeGrid** grid;
    node** shelves;            // array af pointers til hylder
    int shelf_count;           // antal hylder

    node** dropoffs;           // array af pointers til dropoff-punkter
    int dropoff_count;         // antal dropoffs

    node** pickups;            // array af pointers til pickup-noder
    int pickup_count;          // antal pickups
} Warehouse;
// ===============================================================
// Funktioner til Warehouse
// ===============================================================
Warehouse* createWarehouse(int width, int height);
void generateWarehouseLayout(Warehouse* w, int corridorWidth, int shelfBlockWidth);
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