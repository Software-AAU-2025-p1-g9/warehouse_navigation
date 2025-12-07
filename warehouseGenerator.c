
#include "warehouseGenerator.h"

#include <math.h>

#include "warehouse.h"
#include <stdio.h>
#include <stdlib.h>
// -----------------------------------------
// laver fast array til alle modes og tæller til hvor mange, der er lavet.


// ============================================================
// Hjælpefunktion: Opret en ny graf-node
// ============================================================
node* newNode(int x, int y) { // newnode er lige lidt ligegydig, da den returnerer
    // Vi allokerer direkte et node-array og sætter alt til 0/NULL med calloc
    node* n = calloc(1, sizeof(node)); // calloc sætter alle felter til 0/NULL
    if (!n) return NULL;

    // Felterne x og y er egentlig ligegyldige, men vi sætter dem for reference
    n->x = x;
    n->y = y;

    // Resten nulstilles af calloc, men vi kan også eksplicit sætte dem
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
node** createWarehouse(int width, int height) { // ændre den til ikke at være en struct (warehouse)
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "ERROR: Invalid warehouse size!\n");
        exit(EXIT_FAILURE);
    }

    // Alloker grid som array af pointers til rækker
    node** grid= malloc(sizeof(node*) * height);
    if (!grid) {
        fprintf(stderr, "ERROR: Failed to allocate warehouse grid!\n");
        exit(EXIT_FAILURE);
    }

    for (int y = 0; y < height; y++) {
        grid[y] = calloc(width, sizeof(node)); // calloc nulstiller alt
        if (!grid[y]) {
            fprintf(stderr, "ERROR: Failed to allocate warehouse row %d!\n", y);
            for (int i = 0; i < y; i++) free(grid[i]);
            free(grid);
            exit(EXIT_FAILURE);
        }

        for (int x = 0; x < width; x++) {
            grid[y][x].x = x;
            grid[y][x].y = y;

        }
    }

    return grid; // Shelves/dropoffs/pickups arrays allokeres senere, når vi kender antal
}

// ============================================================
// generateWarehouseLayout
//  Fylder lageret med shelves, dropoffs og pickups
// ============================================================
void generateWarehouseLayout(node** grid, int width, int height, node*** shelves, int* shelf_count, node*** dropoffs, int* dropoff_count, node*** pickups, int* pickup_count, int corridorWidth)
{
    *shelf_count = 0;
    *dropoff_count = 0;
    *pickup_count = 0; // <-- tilføjet fra setPickupPoints

    int x = 0;
    // Første loop: gennem hele bredden, hylde- og korridor-blokke tomme
    while (x < width) {
        // Hylde-blok (tom)
        for (int s = 0; s < 2 && x < width; s++, x++) { }

        // Korridor-blok (tom)
        for (int c = 0; c < corridorWidth && x < width; c++, x++) { }
    }

    // Nu har vi shelf_count klar
    // <-- NYT: malloc shelves-array og error handling
    *shelves = (node**) malloc(sizeof(node*) * (*shelf_count));
    if (!*shelves) {
        fprintf(stderr, "ERROR: Failed to allocate shelves array!\n");
        exit(EXIT_FAILURE);
    }

    int shelves_in_array = 0; // <-- NYT: start indeks til fyldning af shelves

    // Nyt loop: fyld pickups og opdater costs
    x = 0;
    while (x < width) {
        // Hylde-blok
        for (int s = 0; s < 2 && x < width; s++, x++) {
            for (int y = 1; y < height - 1; y++) {
                node* n = &grid[y][x];

                // Tilføj til shelves-array
                (*shelves)[shelves_in_array++] = n; // <-- NYT

                // Opdater costs til predecessors og successors
                for (int i = 0; i < n->neighbour_count; i++) {
                    n->predecessors[i]->cost = INFINITY;
                    n->successors[i]->cost = INFINITY;
                }
            }
        }

        // Korridor-blok
        for (int c = 0; c < corridorWidth && x < width; c++, x++) {
            for (int y = 1; y < height - 1; y++) {
                node* n = &grid[y][x];

                // Tilføj til pickups direkte (fra setPickupPoints)
                (*pickups)[(*pickup_count)++] = n;

                // Opdater costs til predecessors og successors
                for (int i = 0; i < n->neighbour_count; i++) {
                    edge* e = n->predecessors[i];
                    if (!e) continue;
                    float dx = (float)(n->x - e->source->x);
                    float dy = (float)(n->y - e->source->y);
                    e->cost = sqrtf(dx*dx + dy*dy);
                }
                for (int i = 0; i < n->neighbour_count; i++) {
                    edge* e = n->successors[i];
                    if (!e) continue;
                    float dx = (float)(n->x - e->dest->x);
                    float dy = (float)(n->y - e->dest->y);
                    e->cost = sqrtf(dx*dx + dy*dy); // float version
                }
            }
        }
    }

    // Dropoff på tilfældig walkable position i korridor
    int drop_x = corridorWidth; // eksempel: første korridorkolonne
    int drop_y = height / 2;
    (*dropoffs)[(*dropoff_count)++] = &grid[drop_y][drop_x];
}
// ============================================================
// printWarehouse
//  Printer lager med symboler: S, D, P, .
// ============================================================
void printWarehouse(node*** grid, int width, int height, node** shelves, int shelf_count, node** dropoffs, int dropoff_count, node** pickups, int pickup_count) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            node* c = &grid[y][x];
            int is_shelf = 0;
            for (int s = 0; s < shelf_count; s++) {
                if (shelves[s] == c) { is_shelf = 1; break; }
            }
            if (is_shelf) printf("S");
            else if (c == dropoffs[0]) printf("D");
            else {
                int is_pickup = 0;
                for (int p = 0; p < pickup_count; p++) {
                    if (pickups[p] == c) { is_pickup = 1; break; }
                }
                if (is_pickup) printf("P");
                else printf(".");
            }
        }
        printf("\n");
    }
}

// ============================================================
// createGraphFromWarehouse ( create_graph=
//  Opretter graf af alle walkable felter med 8 naboer (inkl. diagonaler)
// ============================================================
void create_graph(int width, int height, node*** grid, node** shelves, int shelf_count, edge* edges, int* edge_count) {
    // alloker selve arrayet.
    *grid = malloc(sizeof(node*) * height);
    if (!*grid) {
        fprintf(stderr, "ERROR: Failed to allocate grid rows GG!\n");
        exit(EXIT_FAILURE);
    }
    // 1. Opret noder for alle walkable felter (ikke hylder) Få kigget på dette også
    for (int y = 0; y < height; y++) {
        (*grid)[y] = calloc(width, sizeof(node));
        if (!(*grid)[y]) {
            fprintf(stderr, "ERROR: Failed to allocate row GG %d!\n", y);
            // frigør allerede allokerede rækker.
            for (int i = 0; i < y; i++) free((*grid)[i]);
            free(*grid);
            exit(EXIT_FAILURE);
        }
        for (int x = 0; x < width; x++) {
            (*grid)[y][x].y = y;
            (*grid)[y][x].x = x;
        }
    }

    // 2. Retninger for 8 naboer (ortogonale + diagonale)
    int dirs[8][2] = {
        { 0,-1}, { 0, 1}, {-1, 0}, { 1, 0},  // op, ned, venstre, højre
        {-1,-1}, {-1, 1}, { 1,-1}, { 1, 1}   // diagonaler
    };

    // 3. Opret edges for hver node
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                node n = *grid[y][x];

                // Alloker arrays til successorer og predecessorer
                n.successors    = malloc(sizeof(edge*) * MAX_EDGES);
                n.predecessors  = malloc(sizeof(edge*) * MAX_EDGES);
                n.neighbour_count = 0;

                for (int d = 0; d < MAX_EDGES; d++) { // ER DETTE LAVET RIGTIGT DOBBELT TJEK!!!
                    int neighbour_x = n.x + dirs[d][0];
                    int neighbour_y = n.y + dirs[d][1];

                    // Tjek grænser
                    if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= width || neighbour_y >= height)
                        continue;

                    node* neighbour_node = &(*grid)[neighbour_y][neighbour_x];


                    // Opret edge
                    edge* e = &edges[(*edge_count)++];
                    e->source = grid[y][x];
                    e->dest   = neighbour_node;
                    e->cost   = (d < 4) ? 1.0 : 1.414; // diagonaler koster √2

                    n.successors[n.neighbour_count++] = e;
                    neighbour_node->predecessors[neighbour_node->neighbour_count++] = e;
                }
                *grid[y][x] = n;
            }
        }

}

// ============================================================
// freeGraph
//  Frigør kun arrayet af node-pointers
// ============================================================
void freeGraph( node** list, int count) {
    if (!list) return;
    free(list);
}

// ============================================================
// freeWarehouse
//  Frigør grid og arrays
// ============================================================
void freeWarehouse(node*** grid, int width, int height, node** shelves, node** dropoffs, node** pickups) {
    if (!grid) return;

    for (int y = 0; y < height; y++)
        free(grid[y]);
    free(grid);

    free(shelves);
    free(dropoffs);
    free(pickups);
}