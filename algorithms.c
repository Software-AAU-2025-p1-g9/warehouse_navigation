//
// Created by Alex on 26-11-2025.
//

#include "warehouse.h"
#include "algorithms.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void insert_to_priority_queue_after_node(priority_queue_element* n_1, priority_queue_element* n_2, priority_queue* queue);

double h(node n, node goal_node) {
    return abs(n.x - goal_node.x) + abs(n.y - goal_node.y);
}


key calculate_key(node n, node goal_node, int goal_pos){
    return (key) {min(n.g[goal_pos], n.rhs[goal_pos]) + h(n, goal_node), min(n.g[goal_pos], n.rhs[goal_pos])};
}

int is_key_smaller(key k, key l) {
    if (k.k_1 != l.k_1) {
        return k.k_1 < l.k_1;
    }
    else {
        return k.k_2 < l.k_2;
    }
}

void insert_to_priority_queue(node* n, key k, priority_queue* queue) {
    priority_queue_element* new_element = (priority_queue_element*) malloc(sizeof(priority_queue_element));
    if (new_element == NULL) {
        printf("Out of memory :(");
        exit(EXIT_FAILURE);
    }
    new_element->node = n;
    new_element->key = k;

    priority_queue_element* current_element = queue->first;
    if (current_element == NULL || is_key_smaller(new_element->key, current_element->key)) {
        insert_to_priority_queue_after_node(NULL, new_element, queue);
        return;
    }
    while (current_element->next != NULL && is_key_smaller(current_element->next->key, new_element->key)) {
        current_element = current_element->next;
    }
    insert_to_priority_queue_after_node(current_element, new_element, queue);
}

/**
 * Indsætter n_1 efter n_2 i queue
 * @param n_1 pointer til element i queue eller NULL
 * @param n_2 pointer til element der skal indsættes
 * @param queue priority queue
 */
void insert_to_priority_queue_after_node(priority_queue_element* n_1, priority_queue_element* n_2, priority_queue* queue) {
    if (n_1 == NULL) {
        n_2->next = queue->first;
        queue->first = n_2;
    }
    else {
        n_2->next = n_1->next;
        n_1->next = n_2;
    }
}


void initialize_lpa_star(node* warehouse, int size_x, int size_y, node* start_node, node* goal_node, priority_queue* priority_queues){
    int goal_pos = node_pos(size_x, goal_node->x, goal_node->y);
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            int pos = node_pos(size_x, x, y);
            warehouse[pos].g[goal_pos] = INFINITY;
            warehouse[pos].rhs[goal_pos] = INFINITY;
        }
    }
    goal_node->g[goal_pos] = INFINITY;
    goal_node->rhs[goal_pos] = INFINITY;
    key start_key = calculate_key(*start_node, *goal_node, goal_pos);
    priority_queues[goal_pos].first = &(priority_queue_element) {start_key, start_node, NULL};
}

void update_node_lpa_star(int size_x, node* n, node* start, node* goal, priority_queue* queue) {
    int g_pos = node_pos(size_x, goal->x, goal->y);
    if (n != start) {
        //Finder den mindste g + c for alle predecessors
        float min_predecessor_sum = INFINITY;
        for (int i = 0; i < n->neighbour_count; i++) {
            float predecessor_distance = n->predecessors[i]->source->g[g_pos] + n->predecessors[i]->cost;
            if (predecessor_distance < min_predecessor_sum) {
                min_predecessor_sum = predecessor_distance;
            }
        }
        n->rhs[g_pos] = min_predecessor_sum;
    }
    /* ------------------------------------------------------------------------------------------------------------------
    if (n in queue)
    {
        queue.remove(n)
    }
    ------------------------------------------------------------------------------------------------------------------ */
    if (n->g[g_pos] != n->rhs[g_pos]) {
        insert_to_priority_queue(n, calculate_key(*n, *goal, g_pos), queue);
    }
}

void lpa_star(node* warehouse, int size_x, int size_y, node start_node, node goal_node){

}


/**
 * Printer g-værdier for alle nodes ud fra en bestem key node. Hvis g er uendelig, printes INF
 * @param warehouse lager-arrayet
 * @param size_x bredden af lageret
 * @param size_y højden af lageret
 * @param key_node den node kortet er defineret ud fra
 */
void print_g(node* warehouse, int size_x, int size_y, node key_node) {
    int key_pos = node_pos(size_x, key_node.x, key_node.y);
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            double node_value = warehouse[node_pos(size_x, x, y)].g[key_pos];
            if (node_value == INFINITY) {
                printf("INF ");
            }
            else {
                printf("%04.1lf ", node_value);
            }
        }
        printf("\n");
    }
}