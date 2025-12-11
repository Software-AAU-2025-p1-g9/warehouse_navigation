#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "algorithms.h"
#include "warehouse.h"
#include "Robot_controller.h"
#include "warehouseGenerator.h"

int main(void) {
	int width, height, corridor_width;
	printf("Width of warehouse:\n");
	scanf("%d", &width);
	printf("Height of warehouse:\n");
	scanf("%d", &height);
	printf("Corridor width:\n");
	scanf("%d", &corridor_width);


	node** warehouse;

	edge* edges;
	int edge_count;

	create_graph(width, height, &warehouse, &edges, &edge_count);
	node** shelves;
	int shelf_count;

	int pick_up_count = width * height;
	node* pick_up_points[pick_up_count];

	int drop_off_count = width * height;
	node* drop_off_points[drop_off_count];

	map_data map_datas[width * height];
	for (int i = 0; i < width * height; i++) {
		map_datas[i].priority_queue.first = NULL;
		map_datas[i].key_modifier = 0;
		map_datas[i].last_variable_node = NULL;
	}

	generateWarehouseLayout(warehouse, width, height, &shelves, &shelf_count, drop_off_points, &drop_off_count, pick_up_points, &pick_up_count, corridor_width);

	/*
	edge** path;
	int path_len;
	int map_id = node_pos(width, 49, 0);
	initialize_d_star_lite(warehouse, width, height, &warehouse[0][0], &warehouse[0][49], map_datas);
	d_star_lite(&warehouse[0][0], &warehouse[0][49], map_datas, map_id);
	print_g(warehouse, width, height, map_id);
	find_shortest_path_d_star_lite(&path, &path_len, &warehouse[0][0], &warehouse[0][49], map_id);
	printf("\n\n");
	d_star_lite(&warehouse[49][49], &warehouse[0][49], map_datas, map_id);
	print_g(warehouse, width, height, map_id);
	find_shortest_path_d_star_lite(&path, &path_len, &warehouse[49][49], &warehouse[0][49], map_id);
	printf("\n\n");
	map_id = node_pos(width, 2, 2);
	initialize_d_star_lite(warehouse, width, height, &warehouse[1][1], &warehouse[2][2], map_datas);
	d_star_lite(&warehouse[1][1], &warehouse[2][2], map_datas, map_id);
	print_g(warehouse, width, height, map_id);
	find_shortest_path_d_star_lite(&path, &path_len, &warehouse[1][1], &warehouse[2][2], map_id);
	printf("\n\n");
	map_id = node_pos(width, 0, 2);
	initialize_d_star_lite(warehouse, width, height, &warehouse[0][0], &warehouse[2][0], map_datas);
	d_star_lite(&warehouse[0][0], &warehouse[2][0], map_datas, map_id);
	print_g(warehouse, width, height, map_id);
	find_shortest_path_d_star_lite(&path, &path_len, &warehouse[0][0], &warehouse[2][0], map_id);
	printf("\n\n");
	d_star_lite(&warehouse[9][9], &warehouse[2][0], map_datas, map_id);
	print_g(warehouse, width, height, map_id);
	find_shortest_path_d_star_lite(&path, &path_len, &warehouse[9][90], &warehouse[2][0], map_id);
	printf("\n\n");
	*/

	int order_amount, orders_assigned = 0;
	printf("Order amount: \n");
	scanf("%d", &order_amount);
	order orders[order_amount];
	OrderRandomizer(order_amount, orders, pick_up_points, pick_up_count, drop_off_points, drop_off_count, shelves, shelf_count);

	enum algorithm algorithm;
	printf("Algorithm?\n0 - A*\n1 - LPA*\n2 - D* Lite\n");
	scanf("%d", &algorithm);

	Robot r;
	r.has_order = 0;
	r.current_node = &warehouse[0][0];
	r.goal_1 = NULL;
	r.goal_2 = NULL;
	r.path = NULL;
	r.idle = 0;

	float global_time = 0;
	while (r.idle == 0) {
		if (r.has_order == 0) {
			if (orders_assigned < order_amount) {
				assign_robot_order(&r, orders[orders_assigned++]);
				switch (algorithm) {
					case D_STAR_LITE:
						assign_robot_path_d_star_lite(&r, global_time, warehouse, width, height, r.goal_1, map_datas);
						break;
					case LPA_STAR:
						assign_robot_path_lpa_star(&r, global_time, warehouse, width, height, r.goal_1, map_datas);
						break;
					default:
						assign_robot_path(&r, global_time, warehouse, node_pos(width, r.current_node->x, r.current_node->y), height, width, r.goal_1->x, r.goal_1->y);
				}
			}
			else {
				free(r.path);
				r.idle = 1;
				continue;
			}
		}
		move_robot(&r, &global_time);
		if (r.current_node == r.goal_1) {
			switch (algorithm) {
				case D_STAR_LITE:
					assign_robot_path_d_star_lite(&r, global_time, warehouse, width, height, r.goal_2, map_datas);
					break;
				case LPA_STAR:
					assign_robot_path_lpa_star(&r, global_time, warehouse, width, height, r.goal_2, map_datas);
					break;
				default:
					assign_robot_path(&r, global_time, warehouse, node_pos(width, r.current_node->x, r.current_node->y), height, width, r.goal_2->x, r.goal_2->y);

			}
		}
		if (r.current_node == r.goal_2) {
			r.has_order = 0;
		}
	}

	printf("The robot completed the orders in %f04.1 time\n", global_time);
}