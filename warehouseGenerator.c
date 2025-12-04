
#include "warehouseGenerator.h"
#include <stdio.h>
#include <stdlib.h>
// -----------------------------------------
// laver fast array til alle modes og tæller til hvor mange, der er lavet.
#define MAX_NODES 10000
static node nodePool[MAX_NODES];
static int nodeCount = 0;

// ============================================================
// Hjælpefunktion: Opret en ny graf-node
// ============================================================
node* newNode(int x, int y) {
    if (nodeCount >= MAX_NODES) return NULL;

    node* n = &nodePool[nodeCount++];
    n->neighbour_count = 0;
    n->successors = NULL;
    n->predecessors = NULL;
    n->g = NULL;
    n->h = NULL;
    n->rhs = NULL;

    return n;
}

// ============================================================
// createWarehouse
//  Opretter lagerets grid af NodeGrid.
// ============================================================
Warehouse* createWarehouse(int width, int height) {
    if (width <= 0 || height <= 0) return NULL;

    Warehouse* w = malloc(sizeof(Warehouse));
    if (!w) return NULL;

    w->width = width;
    w->height = height;
    w->shelf_count = 0;
    w->dropoff_count = 0;
    w->pickup_count = 0;

    // Alloker grid
    w->grid = malloc(sizeof(NodeGrid*) * height);
    if (!w->grid) { free(w); return NULL; }

    for (int y = 0; y < height; y++) {
        w->grid[y] = malloc(sizeof(NodeGrid) * width);
        for (int x = 0; x < width; x++) {
            w->grid[y][x].x = x;
            w->grid[y][x].y = y;
            w->grid[y][x].id = y * width + x;
            w->grid[y][x].graphNode = NULL;
        }
    }

    // Alloker arrays til shelves, dropoffs og pickups
    w->shelves = malloc(sizeof(node*) * width * height);
    w->dropoffs = malloc(sizeof(node*) * width * height);
    w->pickups = malloc(sizeof(node*) * width * height);

    return w;
}

// ============================================================
// generateWarehouseLayout
//  Fylder lageret med shelves og dropoffs
// ============================================================
void generateWarehouseLayout(Warehouse* w, int corridorWidth, int shelfBlockWidth) {
    int x = 0;
    while (x < w->width) {
        // Hylde-blok
        for (int s = 0; s < shelfBlockWidth && x < w->width; s++, x++) {
            for (int y = 1; y < w->height - 1; y++) {
                node* n = newNode(x, y);
                w->shelves[w->shelf_count++] = n;
                w->grid[y][x].graphNode = n;
            }
        }
        // Korridor-blok (tom)
        for (int c = 0; c < corridorWidth && x < w->width; c++, x++) { }
    }

    // Dropoff i midten
    int mx = w->width / 2;
    int my = w->height / 2;
    node* dropNode = newNode(mx, my);
    w->dropoffs[w->dropoff_count++] = dropNode;
    w->grid[my][mx].graphNode = dropNode;
}

// ============================================================
// setPickupPoints
//  Tilføj alle walkable felter som pickup points
// ============================================================
void setPickupPoints(Warehouse* w) {
    w->pickup_count = 0;

    for (int y = 0; y < w->height; y++) {
        for (int x = 0; x < w->width; x++) {
            NodeGrid* c = &w->grid[y][x];
            if (!c->graphNode) c->graphNode = newNode(c->x, c->y);

            int is_shelf = 0;
            for (int s = 0; s < w->shelf_count; s++) {
                if (w->shelves[s] == c->graphNode) { is_shelf = 1; break; }
            }

            if (!is_shelf) {
                w->pickups[w->pickup_count++] = c->graphNode;
            }
        }
    }

    printf("Total pickup points: %d\n", w->pickup_count);
}

// ============================================================
// printWarehouse
//  Printer lager med symboler: S, D, P, .
// ============================================================
void printWarehouse(Warehouse* w) {
    if (!w) return;

    for (int y = 0; y < w->height; y++) {
        for (int x = 0; x < w->width; x++) {
            NodeGrid* c = &w->grid[y][x];
            int is_shelf = 0;
            for (int s = 0; s < w->shelf_count; s++) {
                if (w->shelves[s] == c->graphNode) { is_shelf = 1; break; }
            }
            if (is_shelf) printf("S");
            else if (c->graphNode == w->dropoffs[0]) printf("D");
            else {
                int is_pickup = 0;
                for (int p = 0; p < w->pickup_count; p++) {
                    if (w->pickups[p] == c->graphNode) { is_pickup = 1; break; }
                }
                if (is_pickup) printf("P");
                else printf(".");
            }
        }
        printf("\n");
    }
}

// ============================================================
// createGraphFromWarehouse
//  Opretter graf af alle walkable felter
// ============================================================
void createGraphFromWarehouse(Warehouse* w, node*** outList, int* outCount) {
    if (!w) return;

    int max = w->width * w->height;
    node** list = malloc(sizeof(node*) * max);
    int count = 0;

    // Opret noder for alle walkable felter
    for (int y = 0; y < w->height; y++) {
        for (int x = 0; x < w->width; x++) {
            NodeGrid* g = &w->grid[y][x];
            int is_shelf = 0;
            for (int s = 0; s < w->shelf_count; s++) {
                if (w->shelves[s] == g->graphNode) { is_shelf = 1; break; }
            }
            if (is_shelf) continue;

            if (!g->graphNode) g->graphNode = newNode(x, y);
            list[count++] = g->graphNode;
        }
    }

    // Output
    *outList = list;
    *outCount = count;
}

// ============================================================
// freeGraph
//  Frigør kun arrayet af node-pointers
// ============================================================
void freeGraph(node** list, int count) {
    if (!list) return;
    free(list);
}

// ============================================================
// freeWarehouse
//  Frigør grid og arrays
// ============================================================
void freeWarehouse(Warehouse* w) {
    if (!w) return;

    for (int y = 0; y < w->height; y++)
        free(w->grid[y]);
    free(w->grid);

    free(w->shelves);
    free(w->dropoffs);
    free(w->pickups);
    free(w);
}