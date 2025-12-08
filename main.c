#include <math.h>
#include <stdio.h>


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

	generateWarehouseLayout(warehouse, width, height, &shelves, &shelf_count, drop_off_points, &drop_off_count, pick_up_points, &pick_up_count, corridor_width);

	int order_amount;
	printf("Order amount: \n");
	scanf("%d", &order_amount);
	order orders[order_amount];
	OrderRandomizer(order_amount, orders, pick_up_points, pick_up_count, drop_off_points, drop_off_count, shelves, shelf_count);
	printf("Første odre starter i punktet (%d, %d)", orders[0].node_1->x, orders[0].node_1->y); //Debug


	Robot r;
	r.has_order = 0;
	r.goal_2 = &warehouse[0][0];
	assign_robot_path(&r, orders[0], warehouse, node_pos(width, r.goal_2->x, r.goal_2->y), height, width);
}