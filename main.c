#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "warehouse.h"
#include "warehouseGenerator.h"
#include "order_generator.h"
#include "Robot_controller.h"

int main(void) {
	int width, height, corridor_width, robot_count, edge_count, shelf_count, order_amount;
	node** warehouse;
	edge* edges;
	node** shelves;
	enum algorithm algorithm;

	printf("Width of warehouse:\n");
	scanf("%d", &width);
	printf("Height of warehouse:\n");
	scanf("%d", &height);
	printf("Corridor width:\n");
	scanf("%d", &corridor_width);
	printf("Robot count:\n");
	scanf("%d", &robot_count);

	adjustWarehouseSize(&width, height, corridor_width);
	create_graph(width, height, &warehouse, &edges, &edge_count);

	int pick_up_count, drop_off_count;
	node** drop_off_points;
	node** pick_up_points;

	generateWarehouseLayout(warehouse, width, height, &shelves, &shelf_count, &drop_off_points, &drop_off_count, &pick_up_points, &pick_up_count, corridor_width);

	map_data map_datas[width * height];
	for (int i = 0; i < width * height; i++) {
		map_datas[i].priority_queue.first = NULL;
		map_datas[i].key_modifier = 0;
		map_datas[i].last_variable_node = NULL;
	}

	printf("Order amount:\n");
	scanf("%d", &order_amount);
	order* orders = malloc(sizeof(order) * order_amount);
	if (orders == NULL) {
		printf("Not enough memory for orders.\n");
		exit(EXIT_FAILURE);
	}

	unsigned int seed;
	printf("Seed (0 for random seed):\n");
	scanf("%ud", &seed);
	if (seed == 0) {
		seed = (unsigned int)(time(NULL) ^ (uintptr_t)&seed); //mostly unpredictable seed for randomization
	}
	srand(seed);
	order_randomizer(order_amount, orders, pick_up_points, pick_up_count, drop_off_points, drop_off_count, shelves, shelf_count);

	printf("Algorithm:\n0 - A*\n1 - LPA*\n2 - D* Lite\n");
	scanf("%d", &algorithm);

	robot robots[robot_count];
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

	int orders_assigned = 0, idle_counter = 0;
	float global_time = 0;
	clock_t start_time = clock();
	//prøver få den til at loop til alle robotterne er idle, men idk
	while (idle_counter < robot_count) {
		//forsøg på at finde den robot med lavest tid
		robot* r = NULL;
		for (int i = 0; i < robot_count; i++) {
			if (robots[i].idle == 1) continue; // ved ik om det rigtigt, men forsøger at få den til at gå videre hvis den robotten er idle
			if (r == NULL || robots[i].time_at_next_stop < r->time_at_next_stop) {
				r = &robots[i];
			}
		}
		if (r == NULL) {
			printf("Could not find a non-idle robot. %d orders completed.\n", orders_assigned - (robot_count - idle_counter));
			break;
		}

		if (r->has_order == 0) {
			if (orders_assigned < order_amount) {
				assign_robot_order(r, orders[orders_assigned++]);
				assign_robot_path(r, &global_time, warehouse, height, width, r->goal_1, map_datas, algorithm);
			}
			else {
				free(r->path);
				r->idle = 1;
                idle_counter++;
				continue;
			}
		}

		if (r->path_length > 0) {
			move_robot(r, &global_time);
		}

		if (r->current_node == r->goal_1) {
			assign_robot_path(r, &global_time, warehouse, height, width, r->goal_2, map_datas, algorithm);
		}
		if (r->current_node == r->goal_2) {
			r->has_order = 0;
		}
	}
	clock_t total_time = clock() - start_time;

	printf("The robots completed the orders in %.1f time.\n", global_time);
	printf("The process took %.1f seconds.\n", ((float) total_time)/CLOCKS_PER_SEC);
}
