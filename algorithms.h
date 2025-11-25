//
// Created by Alex on 23-11-2025.
//

#ifndef WAREHOUSE_NAVIGATION_ALGORITHMS_H
#define WAREHOUSE_NAVIGATION_ALGORITHMS_H

#endif //WAREHOUSE_NAVIGATION_ALGORITHMS_H

struct priority_queue_element;
typedef struct priority_queue_element priority_queue_element;

struct key {
    double k_1;
    double k_2;
};
typedef struct key key;

struct priority_queue_element{
    key key;
    node* node;
    priority_queue_element* previous;
    priority_queue_element* next;
};

struct priority_queue {
    priority_queue_element* first;
};
typedef struct priority_queue priority_queue;


void initialize_lpa_star(node* warehouse, int size_x, int size_y, node* start_node, node* goal_node, priority_queue* priority_queues);
void lpa_star(node* warehouse, int size_x, int size_y, node start_node, node goal_node);

key calculate_key(node n, node goal_node, int goal_pos);
int is_key_smaller(key k, key l);
void insert_to_priority_queue(node* n, key k, priority_queue* queue);
