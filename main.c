#include <math.h>
#include <stdio.h>


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
		map_datas[i].initialized = 0;
		map_datas[i].key_modifier = 0;
	}

	generateWarehouseLayout(warehouse, width, height, &shelves, &shelf_count, drop_off_points, &drop_off_count, pick_up_points, &pick_up_count, corridor_width);

	int order_amount, orders_assigned = 0;
	printf("Order amount: \n");
	scanf("%d", &order_amount);
	order orders[order_amount];
	OrderRandomizer(order_amount, orders, pick_up_points, pick_up_count, drop_off_points, drop_off_count, shelves, shelf_count);

	Robot r;
	r.has_order = 0;
	r.current_node = &warehouse[0][0];
	r.goal_1 = NULL;
	r.goal_2 = NULL;
	r.idle = 0;

	float global_time = 0;
	while (r.idle == 0) {
		if (r.has_order == 0) {
			if (orders_assigned < order_amount) {
				assign_robot_order(&r, orders[orders_assigned++]);
				assign_robot_path_lpa(&r, warehouse, width, height, r.goal_1, map_datas);
			}
			else {
				r.idle = 1;
				continue;
			}
		}
		move_robot(&r, &global_time);
		if (r.current_node == r.goal_1) {
			assign_robot_path_lpa(&r, warehouse, width, height, r.goal_2, map_datas);
		}
		if (r.current_node == r.goal_2) {
			r.has_order = 0;
		}
	}
}