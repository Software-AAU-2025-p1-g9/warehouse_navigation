#include "worker.h"
#include <stdlib.h>
#include <time.h>
#include "algorithms.h"
#include "astar.h"

// Runs either LPA*/D* LITE/A* and then call find_shortest_path
void get_shortest_path(node* start_node,
                                      node* goal_node,
                                      int size_x,
                                      int size_y,
                                      node nodes[size_y][size_x],
                                      map_data* map_datas,
                                      enum algorithm alg,
                                      edge*** path,
                                      int* len) {
    *path = NULL;
    *len = 0;

    switch (alg) {
        case LPA_STAR: {
            int map_id = node_pos(size_x, start_node->x, start_node->y);

            if (map_datas[map_id].last_variable_node == NULL) {
                initialize_lpa_star((node**)nodes, size_x, size_y,
                                    start_node, goal_node, map_datas);
            }

            lpa_star(start_node, goal_node, map_datas, map_id);
            find_shortest_path(path, len, start_node, goal_node, map_id);
            break;
        }

        case D_STAR_LITE: {
            int map_id = node_pos(size_x, goal_node->x, goal_node->y);

            if (map_datas[map_id].last_variable_node == NULL) {
                initialize_d_star_lite((node**)nodes, size_x, size_y,
                                       start_node, goal_node, map_datas);
            }

            d_star_lite(start_node, goal_node, map_datas, map_id);
            find_shortest_path_d_star_lite(path, len,
                                           start_node, goal_node, map_id);
            break;
        }

        case A_STAR: {
            int map_id = node_pos(size_x, start_node->x, start_node->y);
            int total_area = size_x * size_y;

            reset_g((node**)nodes, size_x, size_y, map_id);

            astar((node**)nodes,
                  start_node->x, start_node->y,
                  goal_node->x, goal_node->y,
                  map_id, total_area);

            find_shortest_path(path, len, start_node, goal_node, map_id);
            break;
        }
    }
}

/* This function generate a route using edges
 * and create a test route for these "workers" */
void generate_simple_loop_route(worker* w,
                                int size_y, int size_x,
                                node nodes[size_y][size_x],
                                map_data* map_data,
                                enum algorithm alg) {
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

    // Saves 3 stops
    w->stops[0] = A;
    w->stops[1] = B;
    w->stops[2] = C;

    // Goes from A -> B by finding edges between nodes
    int len_AB = 0;
    edge** path_AB = NULL;

    get_shortest_path(A, B, size_x, size_y,
                        nodes, map_data, alg,
                        &path_AB, &len_AB);

    // Goes from B -> C by finding edges between nodes
    int len_BC = 0;
    edge** path_BC = NULL;

    get_shortest_path(B, C, size_x, size_y,
                        nodes, map_data, alg,
                        &path_BC, &len_BC);

    // Goes from C -> A by finding edges between nodes
    int len_CA = 0;
    edge** path_CA = NULL;

    get_shortest_path(C, A, size_x, size_y,
                        nodes, map_data, alg,
                        &path_CA, &len_CA);


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

    int path_ref = 0;
    for (int i = 0; i < len_AB; i++) w->route[path_ref++] = path_AB[i];
    for (int i = 0; i < len_BC; i++) w->route[path_ref++] = path_BC[i];
    for (int i = 0; i < len_CA; i++) w->route[path_ref++] = path_CA[i];

    free(path_AB);
    free(path_BC);
    free(path_CA);

    for (int i = 0; i < NUM_STOPS; i++) {
        w->stay_time[i] = (float)((rand() % 20) + 1);
    }
}