//
// Created by Alex on 26-11-2025.
//

#ifndef WAREHOUSE_NAVIGATION_ALGORITHMS_H
#define WAREHOUSE_NAVIGATION_ALGORITHMS_H

void print_g(node* warehouse, int size_x, int size_y, node key_node);
void find_shortest_path(edge*** path, int* path_length, node* start_node, node* goal_node, int map_id);
#endif //WAREHOUSE_NAVIGATION_ALGORITHMS_H