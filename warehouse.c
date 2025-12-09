#include "warehouse.h"
#include <stdlib.h>
#include <time.h>

// Converts (x, y) grid coordinates into a single integer
int node_pos(int size_x, int x, int y) {
    return y * size_x + x;
}

/* Find edges that goes from "from" -> "to"
 * and returns a pointer if found otherwise returns nothing/NULL */
edge* find_edge(node* from, node* to) {
    if (from == NULL || from->successors == NULL) {
            return NULL;
        }

    /* Looks at all neighbouring edges
     * And if one of the edges goes to the correct destination, it will return that edge */
    for (int i = 0; i < from->neighbour_count; i++) {
        edge* e = from->successors[i];
        if (e != NULL && e->dest == to) {
            return e;
        }
    }

    return NULL;
}

/* This function generate a route using edges
 * and create a test route for these "workers" */
void generate_simple_loop_route(worker* w,
                                int size_y, int size_x,
                                node nodes[size_y][size_x]) {
    w->position = 0;

    // Picks 3 random coordinates
    int ax = rand() % size_x;
    int ay = rand() % size_y;

    int bx = rand() % size_x;
    int by = rand() % size_y;

    int cx = rand() % size_x;
    int cy = rand() % size_y;

    node* A = &nodes[ay][ax];
    node* B = &nodes[by][bx];
    node* C = &nodes[cy][cx];

    // Will find edges between nodes
    edge* AB = find_edge(A, B);
    edge* BC = find_edge(B, C);
    edge* CA = find_edge(C, A);

    w->route_length = 3;

    w->route = malloc(w->route_length * sizeof(edge*));

    /* Store the edges in the "workers" route
    *  if any edges return NULL, these edges is not connected */
    w->route[0] = AB;
    w->route[1] = BC;
    w->route[2] = CA;

    for (int i = 0; i < NUM_EDGES; i++) {
        w->stay_time[i] = (float)((rand() % 5) + 1);
    }
}