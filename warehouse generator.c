
#include "warehouse generator.h"
#include <stdio.h>
#include <stdlib.h>
// -----------------------------------------

// Hjælpefunktion: Opret en ny graf-node chat sagde det var godt at have den med.
// ============================================================
node* newNode(int x, int y) {
    node* n = malloc(sizeof(node));
    if (!n) return NULL;

    // Tillad skrivning til const.
    // // startede med at skrive n->x = x, men hvis x er const, bruger vi denne metode:
    // *(int*)&n->x = x betyder:
    // - &n->x: Få adressen på x-feltet
    // - (int*): Behandl denne adresse som en pointer til en int
    // - *: Skriv til denne adresse
    *(int*)&n->x = x;   // sæt x-koordinat
    *(int*)&n->y = y;   // sæt y-koordinat

    n->neighbour_count = 0; // samlet antal naboer
    n->successor_count = 0; // antal udgående edges
    n->predecessor_count = 0; // antal indgående edges

    n->successors = malloc(sizeof(edge*) * 8);
    n->predecessors = malloc(sizeof(edge*) * 8);

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

    w->width = width; // gem lagerets bredde
    w->height = height; // gem lagerests højde
    w->pickup = NULL;

    // Alloker grid
    w->grid = malloc(sizeof(NodeGrid*) * height);
    if (!w->grid) {
        free(w);
        return NULL;
    }

    for (int y = 0; y < height; y++) {
        w->grid[y] = malloc(sizeof(NodeGrid) * width);
        for (int x = 0; x < width; x++) {
            NodeGrid* c = &w->grid[y][x];
            c->x = x;
            c->y = y;
            c->id = y * width + x;

            c->isShelf = 0;
            c->isDropoff = 0;
            c->isPickup = 0;
            c->hasItem = 0;
            c->graphNode = NULL;
        }
    }

    return w;
}

// ============================================================
// generateFishbone
//  Laver klassisk fishbone-layout.
//  Inkluderer robotgang hele vejen rundt om lageret.
// ============================================================
void generateFishbone(Warehouse* w, int corridorWidth, int shelfBlockWidth) {
    if (!w) return;

    // Nulstil alt
    for (int y = 0; y < w->height; y++) // start i venstre side og fortsæt indtil vi nået til højre side
        for (int x = 0; x < w->width; x++)
            w->grid[y][x].isShelf = w->grid[y][x].isDropoff = w->grid[y][x].isPickup = w->grid[y][x].hasItem = 0;

    // Byg fishbone
    int x = 0;
    while (x < w->width) {
        // Hylde-blok
        for (int s = 0; s < shelfBlockWidth && x < w->width; s++, x++) {
            // Gør hele kolonnen til hylder (undtagen første og sidste række)
            for (int y = 1; y < w->height - 1; y++)
                w->grid[y][x].isShelf = 1;
        }
        // Korridor-blok
        for (int c = 0; c < corridorWidth && x < w->width; c++, x++) { }
    }

    // Dropoff i midten
    int mx = w->width / 2; // findt midten af bredden
    int my = w->height / 2; // find midten af højden
    if (mx >= 0 && mx < w->width && my >= 0 && my < w->height)
        w->grid[my][mx].isDropoff = 1;

    // Korridor rundt om kanten
    for (int xx = 0; xx < w->width; xx++) {
        w->grid[0][xx].isShelf = 0; //øverste række
        w->grid[w->height-1][xx].isShelf = 0; // nederste række
    }
    for (int yy = 0; yy < w->height; yy++) {
        w->grid[yy][0].isShelf = 0; // venste kolonne
        w->grid[yy][w->width-1].isShelf = 0; // højre kolonne
    }
}

// ============================================================
// autoPlaceItems
//  Lægger "count" varer tilfældigt på hylder.
// ============================================================
void autoPlaceItems(Warehouse* w, int count) {
    if (!w) return;
    int placed = 0;
    while (placed < count) {
        int x = rand() % w->width;
        int y = rand() % w->height;

        NodeGrid* c = &w->grid[y][x];
        if (c->isShelf && !c->hasItem) {
            c->hasItem = 1;
            placed++;
        }
    }
}

// ============================================================
// setPickupPoint
//  Finder første frie felt og sætter robot-pickup.
// ============================================================
void setPickupPoint(Warehouse* w) {
    if (!w) return;

    for (int y = 0; y < w->height; y++) {
        for (int x = 0; x < w->width; x++) {
            NodeGrid* c = &w->grid[y][x];
            if (!c->isShelf) {
                c->isPickup = 1;
                // Sørg for graf-node findes
                if (!c->graphNode) c->graphNode = newNode(c->x, c->y);
                w->pickup = c->graphNode;
                printf("Pickup point placed at (%d,%d)\n", x, y);
                return;
            }
        }
    }
}

// ============================================================
// printWarehouse
//  Printer lager med symboler: S, D, P, I, .
// ============================================================
void printWarehouse(Warehouse* w) {
    if (!w) return;

    for (int y = 0; y < w->height; y++) {
        for (int x = 0; x < w->width; x++) {
            NodeGrid* c = &w->grid[y][x];
            if (c->isDropoff) printf("D");
            else if (c->isPickup) printf("P");
            else if (c->hasItem) printf("I");
            else if (c->isShelf) printf("S");
            else printf(".");
        }
        printf("\n");
    }
}

// ============================================================
// createGraphFromWarehouse
//  Opretter graf af alle walkable felter (8 naboer, diagonal OK)
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
            if (g->isShelf) continue;

            if (!g->graphNode) g->graphNode = newNode(x, y);
            list[count++] = g->graphNode;
        }
    }

    // Retninger (4 ortogonal + 4 diagonal)
    int dirs[8][2] = {
        {0,-1},{0,1},{-1,0},{1,0}, // 4 hovedretninger
        {-1,-1},{-1,1},{1,-1},{1,1} // 4 diagonale retninger
    };

    // Lav edges
    for (int i = 0; i < count; i++) {
        node* n = list[i];
        // Tjek alle 8 mulige retninger fra denne node
        for (int d = 0; d < 8; d++) {
            int nx = n->x + dirs[d][0]; // naboens x-koordinat
            int ny = n->y + dirs[d][1]; // naboens y-koordinat

            if (nx < 0 || ny < 0 || nx >= w->width || ny >= w->height) continue;
            NodeGrid* g = &w->grid[ny][nx];
            if (g->isShelf) continue;

            if (d >= 4) { // diagonaler
                if (w->grid[n->y][ny].isShelf || w->grid[ny][n->x].isShelf) continue;
            }

            // opret en ny edgde mellem nodes
            edge* e = malloc(sizeof(edge));
            e->source = n;
            e->dest = g->graphNode;
            e->cost = (d < 4) ? 1.0 : 1.414;

            n->successors[n->successor_count++] = e;
            g->graphNode->predecessors[g->graphNode->predecessor_count++] = e;
        }
    }

    *outList = list;
    *outCount = count;
}

// ============================================================
// freeGraph
//  Frigør alle nodes og edges i grafen
// ============================================================
void freeGraph(node** list, int count) {
    if (!list) return;

    for (int i = 0; i < count; i++) {
        node* n = list[i];
        for (int e = 0; e < n->successor_count; e++)
            free(n->successors[e]);

        free(n->successors);
        free(n->predecessors);
        free(n);
    }

    free(list);
}

// ============================================================
// freeWarehouse
//  Frigør grid og lager-strukturen
// ============================================================
void freeWarehouse(Warehouse* w) {
    if (!w) return;

    for (int y = 0; y < w->height; y++)
        free(w->grid[y]);
    free(w->grid);
    free(w);
}
// ============================================================


