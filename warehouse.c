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

/* This function calculate for how long each step takes for these "workers"
 * We look up the corresponding edge and use its stay_time function as the cost */
void calculate_times (worker* w) {
    edge edge_path[MAX_ROUTE];
    int edge_count = 0;

    // If there's only 1 node or less, nothing can be calculated
    if (w->route_length <= 1) {
        if (w->route_length == 1) {
            w->stay_time[0] = 0.0f;
        }
        return;
    }

    /* This function will loop over each step
     * as well as find an edge and connecting the 2 nodes */
    for (int i = 0; i < w->route_length - 1; i++) {
        node* from = w->route[i];
        node* to = w->route[i + 1];

        edge* e = find_edge(from, to);

        /* Store the edge in an array
         * and use the cost of the edges as the movement time
         * but if no edges is found we assign the cost to be 0 */
        if (e != NULL) {
            edge_path[edge_count++] = *e;
            w->stay_time[i] = (float)e->cost;
        } else {
            w->stay_time[i] = 0.0f;
        }
    }

    // Last node won't have any edges
    w->stay_time[w->route_length -1] = 0.0f;
}

/* This function generate a loop route
 * and create a test route for these "workers" */
void generate_simple_loop_route(worker* w, int size_y, int size_x,
                                node nodes[size_y][size_x]) {
    // Reset these "workers" stats
    w->route_length = 0;
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

    // Step 0 from A
    w->route[0] = A;

    // Step 1 from A to B
    w->route[1] = B;

    // Step 2 from B to C
    w->route[2] = C;

    // Step 3 from C back to A
    w->route[3] = A;

    w->route_length = 4;
}