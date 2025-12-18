#include <stdio.h>
#include "worker.h"
#include <stdlib.h>

int stop_node_index(worker* w, node* n) {
    for (int i = 0; i < NUM_STOPS; i++) {
        if (w->stops[i] == n) {
            return i;
        }
    }
    return -1;
}

void restore_worker_backup(worker* w) {
    if (w->backed_up_node == NULL) {
        printf("A worker tried to restore backups but had none\n");
        return;
    }

    node* n = w->backed_up_node;
    for (int i = 0; i < n->neighbour_count; i++) {
        n->predecessors[i]->cost = w->backed_up_costs[i];
    }

    w->backed_up_node = NULL;
}

void backup_and_increase_predecessor_costs(worker* w, float delay_time) {
    if (w->current_node == NULL) {
        printf("Worker is not standing on node\n");
        exit(EXIT_FAILURE);
    }

    w->backed_up_node = w->current_node;

    for (int i = 0; i < w->backed_up_node->neighbour_count; i++) {
        w->backed_up_costs[i] = w->backed_up_node->predecessors[i]->cost;
        w->backed_up_node->predecessors[i]->cost += delay_time;
    }
}



// Runs either LPA*/D* LITE/A* and then call find_shortest_path
void shortest_path(node* start_node, node* goal_node, int size_x, int size_y, node** nodes,
                   map_data* map_datas, enum algorithm alg, edge*** path,int* len) {
    switch (alg) {
        case LPA_STAR: {
            int map_id = node_pos(size_x, start_node->x, start_node->y);

            if (map_datas[map_id].last_variable_node == NULL) {
                initialize_lpa_star((node**)nodes, size_x, size_y,
                                    start_node, goal_node, map_datas);
            }

            //lpa_star(start_node, goal_node, map_datas, map_id, nodes, size_x, size_y); //Print version
            lpa_star(start_node, goal_node, map_datas, map_id); //Fast version

            find_shortest_path(path, len, start_node, goal_node, map_id);
            break;
        }

        case D_STAR_LITE: {
            int map_id = node_pos(size_x, goal_node->x, goal_node->y);

            if (map_datas[map_id].last_variable_node == NULL) {
                initialize_d_star_lite(nodes, size_x, size_y, start_node, goal_node, map_datas);
            }

            //d_star_lite(start_node, goal_node, map_datas, map_id, nodes, size_x, size_y); //Print version
            d_star_lite(start_node, goal_node, map_datas, map_id); //Fast version
            find_shortest_path_d_star_lite(path, len, start_node, goal_node, map_id);
            break;
        }

        default: {
            int map_id = node_pos(size_x, start_node->x, start_node->y);
            int total_area = size_x * size_y;

            reset_g((node**)nodes, size_x, size_y, map_id);

            astar((node**)nodes,
                  start_node->x, start_node->y,
                  goal_node->x, goal_node->y,
                  map_id, total_area);

            find_shortest_path(path, len, start_node, goal_node, map_id);
        }
    }
}

void move_worker(worker* w, float* global_time)
{
    if (w == NULL || global_time == NULL) {
        printf("ERROR: move_worker fik NULL* \n");
        exit(EXIT_FAILURE);
    }
    if (w->route == NULL || w->route_length <= 0) {
        printf("ERROR: worker har ingen rute\n");
        exit(EXIT_FAILURE);
    }
    if (w->current_node == NULL) {
        printf("ERROR: current_node er NULL\n");
        exit(EXIT_FAILURE);
    }

    *global_time = w->time_at_next_stop;
    restore_worker_backup(w);

    if (w->current_edge >= w->route_length) {
        w->current_edge = 0;
    }

    edge* e = w->route[w->current_edge];
    if (e == NULL) {
        printf("ERROR: route[%d] er NULL\n", w->current_edge);
        exit(EXIT_FAILURE);
    }

    float wait_time = 0;
    int stop_index = stop_node_index(w, w->current_node);
    if (stop_index >= 0) {
        wait_time = w->stay_time[stop_index];
    }

    float travel_time = e->cost + wait_time;

    backup_and_increase_predecessor_costs(w, travel_time);

    w->current_node = e->dest;
    w->current_edge++;

    if (w->current_edge >= w->route_length) {
        w->current_edge = 0;
    }

    w->time_at_next_stop = *global_time + travel_time;
}


/* This function generate a route using edges
 * and create a test route for these workers*/
void generate_worker_route(worker* w, int size_y, int size_x, node** nodes,
                           map_data* map_datas, enum algorithm alg) {
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
    int len_AB;
    edge** path_AB = NULL;
    shortest_path(A, B, size_x, size_y, nodes, map_datas, alg, &path_AB, &len_AB);

    // Goes from B -> C by finding edges between nodes
    int len_BC;
    edge** path_BC = NULL;
    shortest_path(B, C, size_x, size_y, nodes, map_datas, alg, &path_BC, &len_BC);

    // Goes from C -> A by finding edges between nodes
    int len_CA;
    edge** path_CA = NULL;
    shortest_path(C, A, size_x, size_y, nodes, map_datas, alg, &path_CA, &len_CA);

    w->route_length = len_AB + len_BC + len_CA;
    if (w->route_length == 0) {
        printf("Worker got a route of zero length\n");
        exit(EXIT_FAILURE);
    }

    w->route = malloc(w->route_length * sizeof(edge*));
    if (w->route == NULL) {
        printf("No memory for worker route\n");
        exit(EXIT_FAILURE);
    }

    int path_ref = 0;
    for (int i = 0; i < len_AB; i++) w->route[path_ref++] = path_AB[i];
    for (int i = 0; i < len_BC; i++) w->route[path_ref++] = path_BC[i];
    for (int i = 0; i < len_CA; i++) w->route[path_ref++] = path_CA[i];

    free(path_AB);
    free(path_BC);
    free(path_CA);

    for (int i = 0; i < NUM_STOPS; i++) {
        w->stay_time[i] = (float)(rand() % 20) + 1;
    }

    w->current_edge = 0;
    w->current_node = w->route[0]->source;
    float first_move_time = w->route[0]->cost;
    backup_and_increase_predecessor_costs(w, first_move_time);

    w->time_at_next_stop = first_move_time;
}