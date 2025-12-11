#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "algorithms.h"
#include "warehouse.h"
#include "Robot_controller.h"
#include "warehouseGenerator.h"

int main(void) {
	int width, height, corridor_width, robot_count;
	printf("Width of warehouse:\n");
	scanf("%d", &width);
	printf("Height of warehouse:\n");
	scanf("%d", &height);
	printf("Corridor width:\n");
	scanf("%d", &corridor_width);
	printf("How many robots do you need?\n");
	scanf("%d", &robot_count);


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

	int order_amount, orders_assigned = 0;
	printf("Order amount: \n");
	scanf("%d", &order_amount);
	order orders[order_amount];
	OrderRandomizer(order_amount, orders, pick_up_points, pick_up_count, drop_off_points, drop_off_count, shelves, shelf_count);

	enum algorithm algorithm;
	printf("Algorithm?\n0 - A*\n1 - LPA*\n2 - D* Lite\n");
	scanf("%d", &algorithm);

	int idle_counter = 0;
	float global_time = 0;

	Robot robots[robot_count];
	//Prøver at reset robotterne til 0
	for (int i = 0; i < robot_count; i++) {
		robots[i].path_pos = 0;
		robots[i].path_length = 0;
		robots[i].path = NULL;
		robots[i].goal_1 = NULL;
		robots[i].goal_2 = NULL;
		robots[i].has_order = 0;
		robots[i].time_at_next_stop = 0;
		robots[i].idle = 0;
		robots[i].current_node = &warehouse[0][i];
	}

	//prøver få den til at loop til alle robotterne er idle, men idk
	while (idle_counter < robot_count) {

		//forsøg på at finde den robot med lavest tid
		Robot* r = NULL;
		for (int i = 0; i < robot_count; i++) {
			if (robots[i].idle == 1) continue; // ved ik om det rigtigt, men forsøger at få den til at gå videre hvis den robotten er idle
			if (r == NULL || robots[i].time_at_next_stop < r->time_at_next_stop) {
				r = &robots[i];
			}
		}

		if (r == NULL) break;
		;
		idle_counter = 0;


		if (r->has_order == 0) {
			if (orders_assigned < order_amount) {
				assign_robot_order(r, orders[orders_assigned++]);
				switch (algorithm) {
					case D_STAR_LITE:
						assign_robot_path_d_star_lite(r, global_time, warehouse, width, height, r->goal_1, map_datas);
						break;
					case LPA_STAR:
						assign_robot_path_lpa_star(r, global_time, warehouse, width, height, r->goal_1, map_datas);
						break;
					default:
						assign_robot_path(r, global_time, warehouse, node_pos(width, r->current_node->x, r->current_node->y), height, width, r->goal_1->x, r->goal_1->y);
				}
			}
			else {
				free(r->path);
				r->idle = 1;
                idle_counter++;
				continue;
			}
		}
		move_robot(r, &global_time);
		if (r->current_node == r->goal_1) {
			switch (algorithm) {
				case D_STAR_LITE:
					assign_robot_path_d_star_lite(r, global_time, warehouse, width, height, r->goal_2, map_datas);
					break;
				case LPA_STAR:
					assign_robot_path_lpa_star(r, global_time, warehouse, width, height, r->goal_2, map_datas);
					break;
				default:
					assign_robot_path(r, global_time, warehouse, node_pos(width, r->current_node->x, r->current_node->y), height, width, r->goal_2->x, r->goal_2->y);

			}
		}
		if (r->current_node == r->goal_2) {
			r->has_order = 0;
		}
	}

	printf("The robot completed the orders in %f04.1 time\n", global_time);
}
