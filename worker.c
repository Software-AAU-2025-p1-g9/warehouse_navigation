#include "worker.h"
#include <stdlib.h>
#include <time.h>
#include "algorithms.h"

/* This function generate a route using edges
 * and create a test route for these "workers" */
void generate_simple_loop_route(worker* w,
                                int size_y, int size_x,
                                node nodes[size_y][size_x]) {
    w->current_edge = 0;

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

    int total_nodes = size_x * size_y;
    map_data map_datas[total_nodes];

    // Goes from A -> B by finding edges between nodes
    int len_AB = 0;
    edge** path_AB = NULL;

    int map_id_AB = node_pos(size_x, A->x, A->y);

    // Initializing LPA* for the route A -> B
    initialize_lpa_star((node**)nodes, size_x, size_y, A, B, map_datas);

    lpa_star(A, B, map_datas, map_id_AB);

    // Find the shortest path using array of edges
    find_shortest_path(&path_AB, &len_AB, A, B, map_id_AB);

    // Goes from B -> C by finding edges between nodes
    int len_BC = 0;
    edge** path_BC = NULL;

    int map_id_BC = node_pos(size_x, B->x, B->y);

    // Initializing LPA* for the route B -> C
    initialize_lpa_star((node**)nodes, size_x, size_y, B, C, map_datas);

    lpa_star(B, C, map_datas, map_id_BC);

    // Find the shortest path using array of edges
    find_shortest_path(&path_BC, &len_BC, B, C, map_id_BC);

    // Goes from C -> A by finding edges between nodes
    int len_CA = 0;
    edge** path_CA = NULL;

    int map_id_CA = node_pos(size_x, C->x, C->y);

    // Initializing LPA* for the route C -> A
    initialize_lpa_star((node**)nodes, size_x, size_y, C, A, map_datas);

    lpa_star(C, A, map_datas, map_id_CA);

    // Find the shortest path using array of edges
    find_shortest_path(&path_CA, &len_CA, C, A, map_id_CA);
    w->stops[0] = A;
    w->stops[1] = B;
    w->stops[2] = C;

    find_shortest_path(&path_AB, &len_AB, A, B, map_id_AB);
    find_shortest_path(&path_BC, &len_BC, B, C, map_id_BC);
    find_shortest_path(&path_CA, &len_CA, C, A, map_id_CA);

    int total_edges = len_AB + len_BC + len_CA;
    w->route_length = total_edges;

    w->route = malloc(w->route_length * sizeof(edge*));
    if (w->route == NULL) {
        w->route_length = 0;
        free(path_AB);
        free(path_BC);
        free(path_CA);
        return;
    }

    int path_idx = 0;
    for (int i = 0; i < len_AB; i++) w->route[path_idx++] = path_AB[i];
    for (int i = 0; i < len_BC; i++) w->route[path_idx++] = path_BC[i];
    for (int i = 0; i < len_CA; i++) w->route[path_idx++] = path_CA[i];

    free(path_AB);
    free(path_BC);
    free(path_CA);

    for (int i = 0; i < NUM_STOPS; i++) {
        w->stay_time[i] = (float)((rand() % 20) + 1);
    }
}