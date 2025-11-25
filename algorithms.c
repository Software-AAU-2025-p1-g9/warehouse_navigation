#include "warehouse.h"
#include "algorithms.h"


#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


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

    //Der tjekkes om der skal placeres på pladsen lige efter comp_element. Hvis ikke, sættes comp_element til den næste i rækken.
    int pos = 0;
    priority_queue_element* comp_element = queue-> first;
    while (comp_element != NULL && is_key_smaller(k, comp_element->key) && comp_element->next != NULL) {
        comp_element = comp_element->next;
        pos++;
    }

    if (pos == 0) {
        //pos er nul hvis priority queue er tom, eller hvis elementet skal sættes på plads nul
        *new_element = (priority_queue_element) {k, n, NULL, queue->first};
        queue->first = new_element;
    }
    else {
        *new_element = (priority_queue_element) {k, n, comp_element, comp_element->next};
        comp_element->next = new_element;
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
    priority_queues[goal_pos].first = &(priority_queue_element) {start_key, start_node, NULL, NULL};
}

void lpa_star(node* warehouse, int size_x, int size_y, node start_node, node goal_node){

}