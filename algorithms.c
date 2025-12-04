//
// Created by Alex on 26-11-2025.
//

#include "warehouse.h"
#include "algorithms.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


edge* lowest_g_predecessor(node* n, int map_id);
void find_shortest_sub_path(edge*** path, int* path_length, int pos, node* start_node, node* goal_node, int map_id);

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
 * Placerer en edge på plads nummer pos i path. Hvis goal ikke er nået, tjekker den den næste i rækken
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
            printf("Der blev fundet en vej med længde %d, men den er for lang", *path_length);
            exit(EXIT_FAILURE);
        }
        return;
    }
    edge* best_neighbour_edge = lowest_g_predecessor(goal_node, map_id);
    if (best_neighbour_edge == NULL) {
        printf("Der kunne ikke findes nogen vej, og lageret sprang i luften :(");
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
 * Returnerer pointer til den edge med lavest g-værdi fra n's predecessors. Hvis den laveste værdi er større end eller lig med n's g-værdi, returneres null.
 * @param n node
 * @param map_id id for kortet der arbejdes i
 * @return edge pointer fra predecessors
 */
edge* lowest_g_predecessor(node* n, int map_id) {
    edge* best_neighbour_edge = NULL;
    float best_neighbour_g = INFINITY;
    for (int i = 0; i < n->neighbour_count; i++) {
        float neighbour_g = n->predecessors[i]->source->g[map_id];
        if (neighbour_g < best_neighbour_g && neighbour_g < n->g[map_id]) {
            best_neighbour_g = neighbour_g;
            best_neighbour_edge = n->predecessors[i];
        }
    }
    return best_neighbour_edge;
}
