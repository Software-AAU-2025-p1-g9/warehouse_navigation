//
// Created by Alex on 12-12-2025.
//

#ifndef WAREHOUSE_NAVIGATION_ALGORITHMS_PRINT_H
#define WAREHOUSE_NAVIGATION_ALGORITHMS_PRINT_H

struct priority_queue_element;
typedef struct priority_queue_element priority_queue_element;

struct key {
    float k_1;
    float k_2;
};
typedef struct key key;

struct priority_queue_element{
    key key;
    node* node;
    priority_queue_element* next;
};

struct priority_queue {
    priority_queue_element* first;
};
typedef struct priority_queue priority_queue;

struct map_data {
    priority_queue priority_queue;
    float key_modifier;
    node* last_variable_node;
};
typedef struct map_data map_data;

void initialize_lpa_star(node** warehouse, int size_x, int size_y, node* start_node, node* goal_node, map_data* map_datas);
void lpa_star(node* start_node, node* goal_node, map_data* map_datas, int map_id, node** warehouse, int size_x, int size_y);
void initialize_d_star_lite(node** warehouse, int size_x, int size_y, node* start_node, node* goal_node, map_data* map_datas);
void d_star_lite(node* start_node, node* goal_node, map_data* map_datas, int map_id, node** warehouse, int size_x, int size_y);
void print_g(node** warehouse, int size_x, int size_y, int map_id);
void print_g_and_rhs(node** warehouse, int size_x, int size_y, int map_id);
void find_shortest_path(edge*** path, int* path_length, node* start_node, node* goal_node, int map_id);
void find_shortest_path_d_star_lite(edge*** path, int* path_length, node* start_node, node* goal_node, int map_id);
#endif //WAREHOUSE_NAVIGATION_ALGORITHMS_PRINT_H