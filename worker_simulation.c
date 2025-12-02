#include <stdio.h>
#include <stdlib.h>
#include "warehouse.h"
#include <time.h>

#define num_workers 5

int main(void) {
    // Create a small grid (3x3 nodes)
    int size_x = 3;
    int size_y = 3;

    // Array of 9 nodes (size_x * size_y)
    node nodes[9];

    // Fill the node array with coordinates
    int idx = 0;
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {

            // Set grid position of the node
            nodes[idx].x = x;
            nodes[idx].y = y;

            /* These will store edges for pathfinding but
            will be useful later on once we have implemented a pathfinding algorithme */
            nodes[idx].neighbour_count = 0;
            nodes[idx].successors = NULL;
            nodes[idx].predecessors = NULL;
            nodes[idx].g = NULL;
            nodes[idx].h = NULL;
            nodes[idx].rhs = NULL;

            // move to the next node in the array
            idx++;
        }
    }

    // This ensures the random nodes we pick change each time we run the program
    srand((unsigned) time(NULL));

    // Create an array of workers
    worker workers[num_workers];

    // Give each worker a random A → B → C → A loop route
    for (int i = 0; i < num_workers; i++) {
        generate_simple_loop_route(&workers[i], nodes, size_x, size_y);
    }

    for (int i = 0; i < num_workers; i++) {
        printf("Workers route (length = %d):\n", i, workers[i].route_length);

        for (int s = 0; s < workers[i].route_length; s++) {
            printf("Step %d: nodes at (%d, %d)\n",
                s,
                workers[i].route[s]->x,
                workers[i].route[s]->y);
        }
    }

    return 0;
}
