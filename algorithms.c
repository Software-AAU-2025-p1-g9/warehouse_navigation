//
// Created by Alex on 26-11-2025.
//

#include "warehouse.h"
#include "algorithms.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


float h(node n_1, node n_2);

key calculate_key_lpa_star(node n, node goal_node, int map_id);
int is_key_smaller(key k, key l);

void insert_to_priority_queue(node* n, key k, priority_queue* queue);
void insert_to_priority_queue_after_node(priority_queue_element* n_1, priority_queue_element* n_2, priority_queue* queue);
int is_in_priority_queue(node* n, priority_queue* queue);
void remove_from_priority_queue(node* n, priority_queue* queue);
key top_key(priority_queue queue);


/**
 * returnerer den optimistiske afstand mellem to nodes
 * @param n_1 node
 * @param n_2 node
 * @return optimistisk afstand mellem n_1 og n_2
 */
float h(node n_1, node n_2) {
    int x_dist = abs(n_1.x - n_2.x);
    int y_dist = abs(n_1.y - n_2.y);
    float min_dist = (float) min(x_dist, y_dist);
    float max_dist = (float) max(x_dist, y_dist);
    return sqrtf(min_dist * min_dist * 2) + max_dist - min_dist;
}

/**
 *
 * @param n node der skal regnes key for
 * @param goal_node mål-node der skal regnes key ud fra
 * @param map_id id for det kort der arbejdes i
 * @return den udregnede key
 */
key calculate_key_lpa_star(node n, node goal_node, int map_id) {
    return (key) {f_min(n.g[map_id], n.rhs[map_id]) + h(n, goal_node), f_min(n.g[map_id], n.rhs[map_id])};
}

/**
 * tjekker om k er mindre end l
 * @param k key
 * @param l key
 * @return 1 eller 0
 */
int is_key_smaller(key k, key l) {
    if (k.k_1 != l.k_1) {
        return k.k_1 < l.k_1;
    }
    else {
        return k.k_2 < l.k_2;
    }
}

/**
 * Indsætter en node i en priority queue, så den står på den rette sorterede plads
 * @param n pointer til node der skal indsættes
 * @param k key der skal indsættes med hensyn til
 * @param queue pointer til priority queue der skal indsættes i
 */
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

/**
 * Tjekker om en node er i priority queuen
 * @param n node pointer
 * @param queue priority queue pointer
 * @return 1 eller 0
 */
int is_in_priority_queue(node* n, priority_queue* queue) {
    priority_queue_element* element = queue->first;
    while (element != NULL) {
        if (element->node == n) {
            return 1;
        }
        element = element->next;
    }
    return 0;
}

/**
 * Fjerner første instans af n i priority_queue
 * @param n node pointer
 * @param queue priority queue pointer
 */
void remove_from_priority_queue(node* n, priority_queue* queue) {
    priority_queue_element* element = queue->first;
    if (element == NULL) {
        return;
    }
    //Tjekker første plads
    if (element->node == n) {
        priority_queue_element* new_next = element->next;
        free(element);
        queue->first = new_next;
        return;
    }
    //Tjekker resten
    while (element->next != NULL) {
        if (element->next->node == n) {
            priority_queue_element* new_next = element->next->next;
            free(element->next);
            element->next = new_next;
            return;
        }
        element = element->next;
    }
}

key top_key(priority_queue queue) {
    if (queue.first == NULL) {
        return (key) {INFINITY, INFINITY};
    }
    return queue.first->key;
}

/**
 * Initialiserer et "kort", der er bestemt af både start_node og goal_node. Kan ikke bruges hvis man ændrer på nogen af dem senere hen
 * @param warehouse array af array af nodes
 * @param size_x bredde af warehouse
 * @param size_y højde af warehouse
 * @param start_node node pointer
 * @param goal_node node pointer
 * @param queues array af priority queues
 */
void initialize_lpa_star(node** warehouse, int size_x, int size_y, node* start_node, node* goal_node, priority_queue* queues) {
    int map_id = node_pos(size_x, goal_node->x, goal_node->y);
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            warehouse[y][x].g[map_id] = INFINITY;
            warehouse[y][x].rhs[map_id] = INFINITY;
        }
    }
    goal_node->g[map_id] = INFINITY;
    goal_node->rhs[map_id] = INFINITY;
    key start_key = calculate_key_lpa_star(*start_node, *goal_node, map_id);
    queues[map_id].first = &(priority_queue_element) {start_key, start_node, NULL};
}


/**
 * Opdaterer rhs for n, og indsætter til/fjerner fra queue
 * @param n pointer til node der skal opdateres
 * @param start node pointer
 * @param goal node pointer
 * @param queue priority_queue pointer
 * @param map_id id for det kort der arbejdes i
 */
void update_node_lpa_star(node* n, node* start, node* goal, priority_queue* queue, int map_id) {
    if (n != start) {
        //Finder den mindste g + c for alle predecessors
        float min_predecessor_sum = INFINITY;
        for (int i = 0; i < n->neighbour_count; i++) {
            float predecessor_distance = n->predecessors[i]->source->g[map_id] + n->predecessors[i]->cost;
            if (predecessor_distance < min_predecessor_sum) {
                min_predecessor_sum = predecessor_distance;
            }
        }
        n->rhs[map_id] = min_predecessor_sum;
    }
    if (is_in_priority_queue(n, queue))
    {
        remove_from_priority_queue(n, queue);
    }
    if (n->g[map_id] != n->rhs[map_id]) {
        insert_to_priority_queue(n, calculate_key_lpa_star(*n, *goal, map_id), queue);
    }
}

/**
 * Opdaterer warehouse, så den korteste vej fra start_node til goal_node er kortlagt. Kan indtil videre kun bruges hvis start_node og goal_node er samme som dem der blev brugt i initialize_lpa_star
 * @param start_node node pointer
 * @param goal_node node pointer
 * @param queues array af priority_queues
 * @param map_id id for det kort der arbejdes i
 */
void lpa_star(node* start_node, node* goal_node, priority_queue* queues, int map_id){
    priority_queue* queue = &queues[map_id];
    while (is_key_smaller(top_key(*queue), calculate_key_lpa_star(*goal_node, *goal_node, map_id)) || goal_node->rhs[map_id] != goal_node->g[map_id]) {
        node* first_node = queue->first->node;
        remove_from_priority_queue(first_node, queue);
        if (first_node->g[map_id] > first_node->rhs[map_id]) {
            first_node->g[map_id] = first_node->rhs[map_id];
            for (int i = 0; i < first_node->neighbour_count; i++) {
                update_node_lpa_star(first_node->successors[i]->dest, start_node, goal_node, queue, map_id);
            }
        }
        else {
            first_node->g[map_id] = INFINITY;
            //Rækkefølgen her er potentielt en fejlkilde
            for (int i = 0; i < first_node->neighbour_count; i++) {
                update_node_lpa_star(first_node->successors[i]->dest, start_node, goal_node, queue, map_id);
            }
            update_node_lpa_star(first_node, start_node, goal_node, queue, map_id);
        }
    }
}


/**
 * Printer g-værdier for alle nodes ud fra en bestem key node. Hvis g er uendelig, printes INF
 * @param warehouse array af arrays af nodes
 * @param size_x int
 * @param size_y int
 * @param id_node start_node for A* og LPA*. goal_node for D* Lite
 */
void print_g(node* warehouse, int size_x, int size_y, node id_node) {
    int map_id = node_pos(size_x, id_node.x, id_node.y);
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            float node_value = warehouse[node_pos(size_x, x, y)].g[map_id];
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