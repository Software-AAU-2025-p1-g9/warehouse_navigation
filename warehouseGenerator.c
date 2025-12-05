
#include "warehouseGenerator.h"
#include <stdio.h>
#include <stdlib.h>
// -----------------------------------------
// laver fast array til alle modes og tæller til hvor mange, der er lavet.

struct node;  // fortæller, at struct node eksisterer

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
NodeGrid** createWarehouse(int width, int height) { // ændre den til ikke at være en struct (warehouse)
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "ERROR: Invalid warehouse size!\n");
        exit(EXIT_FAILURE);
    }

    // Alloker grid som array af pointers til rækker
    NodeGrid** grid = malloc(sizeof(NodeGrid*) * height);
    if (!grid) {
        fprintf(stderr, "ERROR: Failed to allocate warehouse grid!\n");
        exit(EXIT_FAILURE);
    }

    for (int y = 0; y < height; y++) {
        grid[y] = calloc(width, sizeof(NodeGrid)); // calloc nulstiller alt
        if (!grid[y]) {
            fprintf(stderr, "ERROR: Failed to allocate warehouse row %d!\n", y);
            for (int i = 0; i < y; i++) free(grid[i]);
            free(grid);
            exit(EXIT_FAILURE);
        }

        for (int x = 0; x < width; x++) {
            grid[y][x].x = x;
            grid[y][x].y = y;
            grid[y][x].id = y * width + x;
            grid[y][x].graphNode = NULL;
        }
    }

    return grid; // Shelves/dropoffs/pickups arrays allokeres senere, når vi kender antal
}

// ============================================================
// generateWarehouseLayout
//  Fylder lageret med shelves og dropoffs
// ============================================================
void generateWarehouseLayout(NodeGrid** grid, int width, int height, node*** shelves, int* shelf_count, node*** dropoffs, int* dropoff_count, int corridorWidth, int shelfBlockWidth) {
    *shelf_count = 0;
    *dropoff_count = 0;

    int x = 0;
    while (x < width) {
        // Hylde-blok
        for (int s = 0; s < shelfBlockWidth && x < width; s++, x++) {
            for (int y = 1; y < height - 1; y++) {
                node* n = newNode(x, y);
                (*shelves)[(*shelf_count)++] = n;
                grid[y][x].graphNode = n;
            }
        }
        // Korridor-blok (tom) Få dette rettet.
        for (int c = 0; c < corridorWidth && x < width; c++, x++) { }
    }

    // Dropoff på tilfældig walkable position i korridor
    int dropX = corridorWidth; // eksempel: første korridorkolonne
    int dropY = height / 2;
    node* dropNode = newNode(dropX, dropY); // Sæt den et andet sted.
    (*dropoffs)[(*dropoff_count)++] = dropNode;
    grid[dropY][dropX].graphNode = dropNode;
}

// ============================================================
// setPickupPoints
//  Tilføj alle walkable felter som pickup points (ikke alle felter. bestemt sted i en væg)
// ============================================================
void setPickupPoints(NodeGrid** grid, int width, int height, node*** pickups, int* pickup_count) {
    *pickup_count = 0;

    for (int y = 1; y < height - 1; y++) { // eksempel: kun korridorer
        for (int x = 0; x < width; x++) {
            NodeGrid* c = &grid[y][x];
            if (!c->graphNode) c->graphNode = newNode(c->x, c->y);

            (*pickups)[(*pickup_count)++] = c->graphNode;
        }
    }

    printf("Total pickup points: %d\n", *pickup_count);
}

// ============================================================
// printWarehouse
//  Printer lager med symboler: S, D, P, .
// ============================================================
void printWarehouse(NodeGrid** grid, int width, int height, node** shelves, int shelf_count, node** dropoffs, int dropoff_count, node** pickups, int pickup_count) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            NodeGrid* c = &grid[y][x];
            int is_shelf = 0;
            for (int s = 0; s < shelf_count; s++) {
                if (shelves[s] == c->graphNode) { is_shelf = 1; break; }
            }
            if (is_shelf) printf("S");
            else if (c->graphNode == dropoffs[0]) printf("D");
            else {
                int is_pickup = 0;
                for (int p = 0; p < pickup_count; p++) {
                    if (pickups[p] == c->graphNode) { is_pickup = 1; break; }
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
//  Opretter graf af alle walkable felter med 8 naboer (inkl. diagonaler)
// ============================================================
void createGraphFromWarehouse(int width, int height, NodeGrid** grid, node** shelves, int shelf_count, node*** outList, int* outCount) {
    if (!grid) return;

    int max = width * height;
    node** list = malloc(sizeof(node*) * max);
    int count = 0;

    // 1. Opret noder for alle walkable felter (ikke hylder) Få kigget på dette også
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            NodeGrid* g = &grid[y][x];

            // Spring hylder over
            int is_shelf = 0;
            for (int s = 0; s < shelf_count; s++) {
                if (shelves[s] == g->graphNode) { is_shelf = 1; break; }
            }
            if (is_shelf) continue;

            // Opret node hvis ikke allerede oprettet
            if (!g->graphNode) g->graphNode = newNode(x, y);
            list[count++] = g->graphNode;
        }
    }

    // 2. Retninger for 8 naboer (ortogonale + diagonale)
    int dirs[8][2] = {
        { 0,-1}, { 0, 1}, {-1, 0}, { 1, 0},  // op, ned, venstre, højre
        {-1,-1}, {-1, 1}, { 1,-1}, { 1, 1}   // diagonaler
    };

    // 3. Opret edges for hver node
    for (int i = 0; i < count; i++) {
        node* n = list[i];

        // Alloker arrays til successorer og predecessorer
        n->successors    = malloc(sizeof(edge*) * MAX_EDGES);
        n->predecessors  = malloc(sizeof(edge*) * MAX_EDGES);
        n->neighbour_count = 0;

        for (int d = 0; d < MAX_EDGES; d++) { // ER DETTE LAVET RIGTIGT DOBBELT TJEK!!!
            int nx = n->x + dirs[d][0];
            int ny = n->y + dirs[d][1];

            // Tjek grænser
            if (nx < 0 || ny < 0 || nx >= width || ny >= height)
                continue;

            NodeGrid* ng = &grid[ny][nx];
            if (!ng->graphNode) continue; // Tjek at node findes

            // For diagonaler: tjek at den ikke krydser hylde
            if (d >= 4) { // diagonale retninger
                NodeGrid* n1 = &grid[n->y][ny];  // ortogonal 1
                NodeGrid* n2 = &grid[ny][n->x];  // ortogonal 2
                int blocked = 0;
                for (int s = 0; s < shelf_count; s++) {
                    if (shelves[s] == n1->graphNode || shelves[s] == n2->graphNode) {
                        blocked = 1;
                        break;
                    }
                }
                if (blocked) continue;
            }

            // Opret edge
            edge* e = malloc(sizeof(edge));
            e->source = n;
            e->dest   = ng->graphNode;
            e->cost   = (d < 4) ? 1.0 : 1.414; // diagonaler koster √2

            n->successors[n->neighbour_count++] = e;
            ng->graphNode->predecessors[ng->graphNode->neighbour_count++] = e;
        }
    }

    // 4. Returner pointer til alle nodes og antal
    *outList  = list;
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
void freeWarehouse(NodeGrid** grid, int width, int height, node** shelves, node** dropoffs, node** pickups) {
    if (!grid) return;

    for (int y = 0; y < height; y++)
        free(grid[y]);
    free(grid);

    free(shelves);
    free(dropoffs);
    free(pickups);
}