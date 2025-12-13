#include "worker.h"
#include <stdlib.h>
#include <time.h>
#include "algorithms.h"
#include "astar.h"

int stop_node(worker* w, node* n) {
    if (w == NULL || n == NULL) {
        return -1;
    }
    for (int i = 0; i < NUM_STOPS; i++) {
        if (w->stops[i] == n) {
            return i;
        }
    }
    return -1;
}

// Runs either LPA*/D* LITE/A* and then call find_shortest_path
void shortest_path(node* start_node,
                                      node* goal_node, int size_x, int size_y, node nodes[size_y][size_x],
                                      map_data* map_datas, enum algorithm alg, edge*** path,int* len) {
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

void new_predecessor_costs(node* n, int map_id, float worker_wait,
                                   float* old_costs, int* old_count) {
    *old_count = 0;
    if (!n || !n->predecessors) {
        return;
    }

    for (int i = 0; i < n->neighbour_count; i++) {
        edge* e = n->predecessors[i];
        if (!e) {
            continue;
        }

        old_costs[*old_count] = e->cost;
        (*old_count)++;

        float g_here = (n->g != NULL) ? n->g[map_id] : 0.0f;
        e->cost = g_here + worker_wait;
    }
}

void restore_predecessor_costs(node* n, float* old_costs, int old_count) {
    if (!n || !n->predecessors) {
        return;
    }

    int k = 0;
    for (int i = 0; i < n->neighbour_count && k < old_count; i++) {
        edge* e = n->predecessors[i];
        if (!e) {
            continue;
        }
        e->cost = old_costs[k++];
    }
}
void move_worker(worker* w, float* global_time, int size_x, enum algorithm alg) {

    if (w == NULL || global_time == NULL) {
        return;
    }
    if (w->route == NULL || w->route_length <= 0) {
        return;
    }
    if (w->current_node == NULL) {
        return;
    }

    *global_time = w->time_stop;

    if (w->current_edge >= w->route_length) {
        w->current_edge = 0;
    }

    edge* e = w->route[w->current_edge];
    if (e == NULL) {
        w->current_edge++;
        if (w->current_edge >= w->route_length) {
            w->current_edge = 0;
            return;
        }
    }

    node* from_node = w->current_node;

    float wait_time = 0.0f;
    int stop_ref = stop_node(w, from_node);
    if (stop_ref >= 0) {
        wait_time = w->stay_time[stop_ref];
    }

    int map_id;

    if (alg == D_STAR_LITE) {
        map_id = node_pos(size_x, e->dest->x, e->dest->y);
    } else {
        map_id = node_pos(size_x, from_node->x, from_node->y);
    }

    float old_costs[from_node->neighbour_count];
    int old_count = 0;
    new_predecessor_costs(from_node, map_id, wait_time, old_costs, &old_count);

    w->current_node = e->dest;

    w->current_edge++;
    if (w->current_edge >= w->route_length) {
        w->current_edge = 0;
    }

    restore_predecessor_costs(from_node, old_costs, old_count);

    float next_wait = 0;
    int next_stop = stop_node(w, w->current_node);
    if (next_stop >= 0) {
        next_wait = w->stay_time[next_stop];
    }

    float next_move_time = 0;
    edge* next_edge = w->route[w->current_edge];
    if (next_edge != NULL) {
        next_move_time = next_edge->cost;
    }

    w->time_stop = *global_time + next_wait + next_move_time;
}

/* This function generate a route using edges
 * and create a test route for these "workers" */
void generate_simple_loop_route(worker* w, int size_y, int size_x, node nodes[size_y][size_x],
                                map_data* map_data, enum algorithm alg, float global_time) {
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

    shortest_path(A, B, size_x, size_y,
                        nodes, map_data, alg,
                        &path_AB, &len_AB);

    // Goes from B -> C by finding edges between nodes
    int len_BC = 0;
    edge** path_BC = NULL;

    shortest_path(B, C, size_x, size_y,
                        nodes, map_data, alg,
                        &path_BC, &len_BC);

    // Goes from C -> A by finding edges between nodes
    int len_CA = 0;
    edge** path_CA = NULL;

    shortest_path(C, A, size_x, size_y,
                        nodes, map_data, alg,
                        &path_CA, &len_CA);

    w->route_length = len_AB + len_BC + len_CA;

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

    w->current_edge = 0;

    float first_move = 0;

    if (w->route_length > 0 && w->route[0] != NULL) {
        w->current_node = w->route[0]->source;
        first_move = w->route[0]->cost;
    } else {
        w->current_node = NULL;
    }

    w->time_stop = global_time + first_move;

}