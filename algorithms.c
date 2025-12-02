//
// Created by Alex on 26-11-2025.
//

#include "warehouse.h"
#include "algorithms.h"

#include <math.h>
#include <stdio.h>


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
