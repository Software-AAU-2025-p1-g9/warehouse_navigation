#include "worker.h"
#include <stdlib.h>
#include <time.h>
#include "algorithms.h"

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
    edge** path_AB = NULL;
    edge** path_BC = NULL;
    edge** path_CA = NULL;

    int len_AB = 0;
    int len_BC = 0;
    int len_CA = 0;

    find_shortest_path(&path_AB, &len_AB, A, B, 0);
    find_shortest_path(&path_BC, &len_BC, B, C, 0);
    find_shortest_path(&path_CA, &len_CA, C, A, 0);

    int total_edges = len_AB + len_BC + len_CA;
    w->route_length = total_edges;

    w->route = malloc((size_t)w->route_length * sizeof(edge*));
    if (w->route == NULL) {
        w->route_length = 0;
        return;
    }

    int idx = 0;
    for (int i = 0; i < len_AB; i++) w->route[idx++] = path_AB[i];
    for (int i = 0; i < len_BC; i++) w->route[idx++] = path_BC[i];
    for (int i = 0; i < len_CA; i++) w->route[idx++] = path_CA[i];

    free(path_AB);
    free(path_BC);
    free(path_CA);

    for (int i = 0; i < NUM_TIME; i++) {
        w->stay_time[i] = (float)((rand() % 5) + 1);
    }
}