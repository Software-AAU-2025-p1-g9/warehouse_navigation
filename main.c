#include <stdio.h>


#include "warehouse.h"
#include <Robot_controller.h>


int main(void) {
	node** warehouse;
	edge* edges;
	order* orders;
	node* pick_up_points;
	node* drop_off_points;
	node* shelves;
	int pick_up_count;
	int drop_off_count;
	int shelve_count;
	int order_count = 10;
	int orders_assigned = 0;
	Robot robot;
	float global_time = 0;

// generer lager
	OrderRandomizer(order_count, orders, pick_up_points, pick_up_count, drop_off_points, drop_off_count, shelves,shelve_count);

	// VÆRDIERNE OVENOVER FORVENTES FRA LAGER GENERATOR
/*
	int Order_Amount;
	printf("amount of orders");
	scanf(" %d" , &Order_Amount);
	order Order_Array[Order_Amount];
	OrderRandomizer(Order_Amount, Order_Array, Pickup, PickLength,Dropoff, DropLength, Shelves, ShelveLength);
*/

	while (orders_assigned < order_count) {
		if (!robot.has_order) {
			assign_robot_path(&robot, orders[orders_assigned]);
			orders_assigned++;
		}
		move_robot(&robot, &global_time);
	}

}