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

key calculate_key(node n, node variable_node, float key_modifier, int map_id);
int is_key_smaller(key k, key l);

void insert_to_priority_queue(node* n, key k, priority_queue* queue);
void insert_to_priority_queue_after_node(priority_queue_element* n_1, priority_queue_element* n_2, priority_queue* queue);
int is_in_priority_queue(node* n, priority_queue* queue);
void remove_from_priority_queue(node* n, priority_queue* queue);
key top_key(priority_queue queue);

void update_node_lpa_star(node* n, node* start, node* goal, float key_modifier, priority_queue* queue, int map_id);

edge* lowest_g_predecessor(node* n, int map_id);
edge* lowest_g_successor(node* n, int map_id);
void find_shortest_sub_path(edge*** path, int* path_length, int pos, node* start_node, node* goal_node, int map_id);
void find_shortest_sub_path_d_star_lite(edge*** path, int* path_length, int pos, node* start_node, node* goal_node, int map_id);

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
 * Udregner en key ud fra de givne input. Bruger metode inspireret af D* Lite
 * @param n node der skal regnes key for
 * @param variable_node den variable node der skal regnes key ud fra. For LPA* er det mål, for D* Lite er det start.
 * @param key_modifier key modifier for kortet der arbejdes i.
 * @param map_id id for det kort der arbejdes i
 * @return den udregnede key
 */
key calculate_key(node n, node variable_node, float key_modifier, int map_id) {
    return (key) {f_min(n.g[map_id], n.rhs[map_id]) + h(n, variable_node) + key_modifier,
                  f_min(n.g[map_id], n.rhs[map_id])};
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
        printf("Out of memory :(\n");
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

/**
 * returnerer første key i queue. Hvis queue er tom, returneres (∞,∞)
 * @param queue priority queue
 * @return første key eller (∞,∞)
 */
key top_key(priority_queue queue) {
    if (queue.first == NULL) {
        return (key) {INFINITY, INFINITY};
    }
    return queue.first->key;
}

/**
 * Initialiserer et "kort", der er bestemt af start_node. Må kun bruges en gang
 * @param warehouse array af array af nodes
 * @param size_x bredde af warehouse
 * @param size_y højde af warehouse
 * @param start_node node pointer
 * @param goal_node node pointer
 * @param map_datas array af map_datas
 */
void initialize_lpa_star(node** warehouse, int size_x, int size_y, node* start_node, node* goal_node, map_data* map_datas) {
    int map_id = node_pos(size_x, start_node->x, start_node->y);
    if (map_datas[map_id].last_variable_node != NULL) {
        printf("Kortet med map_id %d er allerede initialiseret\n", map_id);
        exit(EXIT_FAILURE);
    }
    //De næste to linjer er fra D* Lite
    map_datas[map_id].key_modifier = 0;
    map_datas[map_id].last_variable_node = goal_node;

    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            warehouse[y][x].g[map_id] = INFINITY;
            warehouse[y][x].rhs[map_id] = INFINITY;
        }
    }
    start_node->rhs[map_id] = 0;
    key start_key = calculate_key(*start_node, *goal_node, map_datas[map_id].key_modifier, map_id);
    insert_to_priority_queue(start_node, start_key, &map_datas[map_id].priority_queue);
}


/**
 * Opdaterer rhs for n, og indsætter til/fjerner fra queue
 * @param n pointer til node der skal opdateres
 * @param start node pointer
 * @param goal node pointer
 * @param key_modifier den key modifier der gælder for kortet
 * @param queue priority_queue pointer
 * @param map_id id for det kort der arbejdes i
 */
void update_node_lpa_star(node* n, node* start, node* goal, float key_modifier, priority_queue* queue, int map_id) {
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
        insert_to_priority_queue(n, calculate_key(*n, *goal, key_modifier, map_id), queue);
    }
}

/**
 * Opdaterer warehouse, så den korteste vej fra start_node til goal_node er kortlagt. initialize_lpa_star skal være brugt først.
 * @param start_node node pointer
 * @param goal_node node pointer
 * @param map_datas array af map_datas
 * @param map_id id for det kort der arbejdes i
 */
void lpa_star(node* start_node, node* goal_node, map_data* map_datas, int map_id) {
    if (map_datas[map_id].last_variable_node == NULL) {
        printf("Kortet med map_id %d er ikke initialiseret\n", map_id);
        exit(EXIT_FAILURE);
    }

    //key_modifier bliver øget med afstanden mellem sidste mål-node og nuværende. Baseret på D* Lite
    map_datas[map_id].key_modifier += h(*map_datas[map_id].last_variable_node, *goal_node);
    map_datas[map_id].last_variable_node = goal_node;

    float key_modifier = map_datas[map_id].key_modifier;
    priority_queue* queue = &(map_datas[map_id].priority_queue);

    while (is_key_smaller(top_key(*queue), calculate_key(*goal_node, *goal_node, key_modifier, map_id)) || goal_node->rhs[map_id] != goal_node->g[map_id]) {
        //Følgende del er baseret på D* Lite, når den håndterer flytning af start. I dette tilfælde er det ændret, så den håndterer flytning af mål.
        //Hvis top_key er mindre end den burde være efter key_modifier er ændret, lægges den tilbage i køen med sin nye key.
        key old_key = top_key(*queue);
        node* first_node = queue->first->node;
        remove_from_priority_queue(first_node, queue);
        key new_key = calculate_key(*first_node, *goal_node, key_modifier, map_id);

        if (is_key_smaller(old_key, new_key)) {
            insert_to_priority_queue(first_node, new_key, queue);
        }
        //Her slutter D* Lite-delen
        else if (first_node->g[map_id] > first_node->rhs[map_id]) {
            first_node->g[map_id] = first_node->rhs[map_id];
            for (int i = 0; i < first_node->neighbour_count; i++) {
                update_node_lpa_star(first_node->successors[i]->dest, start_node, goal_node, key_modifier, queue, map_id);
            }
        }
        else {
            first_node->g[map_id] = INFINITY;
            //Rækkefølgen her er potentielt en fejlkilde
            for (int i = 0; i < first_node->neighbour_count; i++) {
                update_node_lpa_star(first_node->successors[i]->dest, start_node, goal_node, key_modifier, queue, map_id);
            }
            update_node_lpa_star(first_node, start_node, goal_node, key_modifier, queue, map_id);
        }
    }
}


/**
 * Initialiserer et "kort", der er bestemt af goal_node. Må kun bruges en gang
 * @param warehouse array af array af nodes
 * @param size_x bredde af warehouse
 * @param size_y højde af warehouse
 * @param start_node node pointer
 * @param goal_node node pointer
 * @param map_datas array af map_datas
 */
void initialize_d_star_lite(node** warehouse, int size_x, int size_y, node* start_node, node* goal_node, map_data* map_datas) {
    int map_id = node_pos(size_x, goal_node->x, goal_node->y);
    if (map_datas[map_id].last_variable_node != NULL) {
        printf("Kortet med map_id %d er allerede initialiseret\n", map_id);
        exit(EXIT_FAILURE);
    }
    map_datas[map_id].key_modifier = 0;
    map_datas[map_id].last_variable_node = start_node;

    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            warehouse[y][x].g[map_id] = INFINITY;
            warehouse[y][x].rhs[map_id] = INFINITY;
        }
    }
    goal_node->rhs[map_id] = 0;

    key goal_key = calculate_key(*goal_node, *start_node, map_datas[map_id].key_modifier, map_id);
    insert_to_priority_queue(goal_node, goal_key, &map_datas[map_id].priority_queue);
}

/**
 * Opdaterer rhs for n, og indsætter til/fjerner fra queue
 * @param n pointer til node der skal opdateres
 * @param start node pointer
 * @param goal node pointer
 * @param key_modifier den key modifier der gælder for kortet
 * @param queue priority_queue pointer
 * @param map_id id for det kort der arbejdes i
 */
void update_node_d_star_lite(node* n, node* start, node* goal, float key_modifier, priority_queue* queue, int map_id) {
    if (n != goal) {
        //Finder den mindste g + c for alle predecessors
        float min_successor_sum = INFINITY;
        for (int i = 0; i < n->neighbour_count; i++) {
            float successor_sum = n->successors[i]->dest->g[map_id] + n->successors[i]->cost;
            if (successor_sum < min_successor_sum) {
                min_successor_sum = successor_sum;
            }
        }
        n->rhs[map_id] = min_successor_sum;
    }
    if (is_in_priority_queue(n, queue))
    {
        remove_from_priority_queue(n, queue);
    }
    if (n->g[map_id] != n->rhs[map_id]) {
        insert_to_priority_queue(n, calculate_key(*n, *start, key_modifier, map_id), queue);
    }
}

/**
 * Opdaterer warehouse, så den korteste vej fra start_node til goal_node er kortlagt. initialize_d_star_lite skal være brugt først.
 * @param start_node node pointer
 * @param goal_node node pointer
 * @param map_datas array af map_datas
 * @param map_id id for det kort der arbejdes i
 */
void d_star_lite(node* start_node, node* goal_node, map_data* map_datas, int map_id) {
    if (map_datas[map_id].last_variable_node == NULL) {
        printf("Kortet med map_id %d er ikke initialiseret\n", map_id);
        exit(EXIT_FAILURE);
    }

    map_datas[map_id].key_modifier += h(*map_datas[map_id].last_variable_node, *start_node);
    map_datas[map_id].last_variable_node = start_node;

    float key_modifier = map_datas[map_id].key_modifier;
    priority_queue* queue = &(map_datas[map_id].priority_queue);

    while (is_key_smaller(top_key(*queue), calculate_key(*start_node, *start_node, key_modifier, map_id)) || start_node->rhs[map_id] != start_node->g[map_id]) {
        //Hvis top_key er mindre end den burde være efter key_modifier er ændret, lægges den tilbage i køen med sin nye key.
        key old_key = top_key(*queue);
        node* first_node = queue->first->node;
        remove_from_priority_queue(first_node, queue);
        key new_key = calculate_key(*first_node, *start_node, key_modifier, map_id);

        if (is_key_smaller(old_key, new_key)) {
            insert_to_priority_queue(first_node, new_key, queue);
        }
        else if (first_node->g[map_id] > first_node->rhs[map_id]) {
            first_node->g[map_id] = first_node->rhs[map_id];
            for (int i = 0; i < first_node->neighbour_count; i++) {
                update_node_d_star_lite(first_node->predecessors[i]->source, start_node, goal_node, key_modifier, queue, map_id);
            }
        }
        else {
            first_node->g[map_id] = INFINITY;
            //Rækkefølgen her er potentielt en fejlkilde
            for (int i = 0; i < first_node->neighbour_count; i++) {
                update_node_d_star_lite(first_node->predecessors[i]->source, start_node, goal_node, key_modifier, queue, map_id);
            }
            update_node_d_star_lite(first_node, start_node, goal_node, key_modifier, queue, map_id);
        }
    }
}


/**
 * Printer g-værdier for alle nodes ud fra et bestemt map_id. Hvis g er uendelig, printes INF
 * @param warehouse array af arrays af nodes
 * @param size_x int
 * @param size_y int
 * @param map_id id for det kort der arbejdes i
 */
void print_g(node** warehouse, int size_x, int size_y, int map_id) {
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            float node_value = warehouse[y][x].g[map_id];
            if (node_value == INFINITY) {
                printf("INF  ");
            }
            else {
                printf("%04.1lf ", node_value);
            }
        }
        printf("\n");
    }
}


/**
 * Laver et array af edge pointere, med en dynamisk længde, der er den korteste vej mellem start og goal. Kortet skal være A* / LPA*-korrekt
 * @param path pointer til et array af pointere til edges - output
 * @param path_length pointer til længden af path - output
 * @param start_node pointer til node
 * @param goal_node pointer til node
 * @param map_id id for kortet der arbejdes i
 */
void find_shortest_path(edge*** path, int* path_length, node* start_node, node* goal_node, int map_id) {
    *path_length = 0;
    find_shortest_sub_path(path, path_length, 0, start_node, goal_node, map_id);
}

/**
 * Placerer en edge på plads nummer pos bagfra i path. Hvis goal ikke er nået, tjekker den den næste i rækken
 * @param path pointer til et array af pointere til edges - output
 * @param path_length pointer til længden af path - output
 * @param pos den plads i arrayet (bagfra), den bedste naboedge skal pladseres i
 * @param start_node pointer til node
 * @param goal_node pointer til node
 * @param map_id id for kortet der arbejdes i
 */
void find_shortest_sub_path(edge*** path, int* path_length, int pos, node* start_node, node* goal_node, int map_id) {
    if (goal_node == start_node) {
        *path = malloc(sizeof(edge*) * *path_length);
        if (*path == NULL) {
            printf("Der blev fundet en vej med længde %d, men den er for lang\n", *path_length);
            exit(EXIT_FAILURE);
        }
        return;
    }
    edge* best_neighbour_edge = lowest_g_predecessor(goal_node, map_id);
    if (best_neighbour_edge == NULL) {
        printf("Der kunne ikke findes nogen vej, og lageret sprang i luften :(\n");
        exit(EXIT_FAILURE);
    }
    //Her er path_length lig med pos
    (*path_length)++;
    find_shortest_sub_path(path, path_length, pos + 1, start_node, best_neighbour_edge->source, map_id);
    //Her er path_length længden af det array er blev malloccet
    //indsætter i path bagfra
    (*path)[*path_length - 1 - pos] = best_neighbour_edge;
}


/**
 * Laver et array af edge pointere, med en dynamisk længde, der er den korteste vej mellem start og goal. Kortet skal være D* Lite-korrekt
 * @param path pointer til et array af pointere til edges - output
 * @param path_length pointer til længden af path - output
 * @param start_node pointer til node
 * @param goal_node pointer til node
 * @param map_id id for kortet der arbejdes i
 */
void find_shortest_path_d_star_lite(edge*** path, int* path_length, node* start_node, node* goal_node, int map_id) {
    *path_length = 0;
    find_shortest_sub_path_d_star_lite(path, path_length, 0, start_node, goal_node, map_id);
}

/**
 * Placerer en edge på plads nummer pos i path. Hvis goal ikke er nået, tjekker den den næste i rækken
 * @param path pointer til et array af pointere til edges - output
 * @param path_length pointer til længden af path - output
 * @param pos den plads i arrayet (bagfra), den bedste naboedge skal pladseres i
 * @param start_node pointer til node
 * @param goal_node pointer til node
 * @param map_id id for kortet der arbejdes i
 */
void find_shortest_sub_path_d_star_lite(edge*** path, int* path_length, int pos, node* start_node, node* goal_node, int map_id) {
    if (start_node == goal_node) {
        *path = malloc(sizeof(edge*) * *path_length);
        if (*path == NULL) {
            printf("Der blev fundet en vej med længde %d, men den er for lang\n", *path_length);
            exit(EXIT_FAILURE);
        }
        return;
    }
    edge* best_neighbour_edge = lowest_g_successor(start_node, map_id);
    if (best_neighbour_edge == NULL) {
        printf("Der kunne ikke findes nogen vej, og lageret sprang i luften :(\n");
        exit(EXIT_FAILURE);
    }
    //Her er path_length lig med pos
    (*path_length)++;
    find_shortest_sub_path_d_star_lite(path, path_length, pos + 1, best_neighbour_edge->dest, goal_node, map_id);
    //Her er path_length længden af det array er blev malloccet
    (*path)[pos] = best_neighbour_edge;
}


/**
 * Returnerer pointer til den edge med lavest g-værdi fra n's predecessors. Hvis den laveste værdi er større end eller lig med n's g-værdi, returneres null.
 * @param n node
 * @param map_id id for kortet der arbejdes i
 * @return edge pointer fra predecessors
 */
edge* lowest_g_predecessor(node* n, int map_id) {
    edge* best_neighbour_edge = NULL;
    float best_neighbour_sum = INFINITY;
    for (int i = 0; i < n->neighbour_count; i++) {
        edge* neighbour_edge = n->predecessors[i];
        float neighbour_sum = neighbour_edge->source->g[map_id] + neighbour_edge->cost;
        if (neighbour_sum < best_neighbour_sum) {
            best_neighbour_edge = neighbour_edge;
            best_neighbour_sum = neighbour_sum;
        }
    }
    if (best_neighbour_edge != NULL && best_neighbour_sum == INFINITY) {
        printf("Der blev valgt en uendelig edge");
    }
    return best_neighbour_edge;
}

/**
 * Returnerer pointer til den edge med lavest g-værdi fra n's successors. Hvis den laveste værdi er større end eller lig med n's g-værdi, returneres null.
 * @param n node
 * @param map_id id for kortet der arbejdes i
 * @return edge pointer fra successors
 */
edge* lowest_g_successor(node* n, int map_id) {
    edge* best_neighbour_edge = NULL;
    float best_neighbour_sum = INFINITY;
    for (int i = 0; i < n->neighbour_count; i++) {
        edge* neighbour_edge = n->successors[i];
        float neighbour_sum = neighbour_edge->dest->g[map_id] + neighbour_edge->cost;
        if (neighbour_sum < best_neighbour_sum) {
            best_neighbour_edge = neighbour_edge;
            best_neighbour_sum = neighbour_sum;
        }
    }
    if (best_neighbour_edge != NULL && best_neighbour_sum == INFINITY) {
        printf("Der blev valgt en uendelig edge\n");
    }
    return best_neighbour_edge;
}
