#include <stdio.h>


#include "warehouse.h"


	int main(void) {
		int ShelveLength = 3, DropLength = 3,PickLength = 3;

		node* Shelves[ShelveLength];
		Shelves[0] = &(node){10, 20};
		Shelves[1] = &(node){30, 40};
		Shelves[2] = &(node){50, 60};
		//MIDLERTIDIG OG TILFÆLDIG VÆRDIER FOR ARRAYET

		node* Pickup[DropLength]; //MIDLERTIDIG OG TILFÆLDIG VÆRDIER FOR ARRAYET
		Pickup[0] = &(node){1, 2};
		Pickup[1] = &(node){3, 4};
		Pickup[2] = &(node){5, 6};

		node* Dropoff[PickLength]; //MIDLERTIDIG OG TILFÆLDIG VÆRDIER FOR ARRAYET
		Dropoff[0] = &(node){100, 200};
		Dropoff[1] = &(node){300, 400};
		Dropoff[2] = &(node){500, 600};

		// VÆRDIERNE OVENOVER FORVENTES FRA LAGER GENERATOR

		int Order_Amount;
		printf("amount of orders");
		scanf(" %d" , &Order_Amount);
		order Order_Array[Order_Amount];
		OrderRandomizer(Order_Amount, Order_Array, Pickup, PickLength,Dropoff, DropLength, Shelves, ShelveLength);

		// slet for loop i endeligt version nedenunder kun for test.
		for (int i = 0; i < Order_Amount; i++) {
			printf("Order %d: node_1 = (%d,%d), node_2 = (%d,%d)\n",
					i+1,
				   Order_Array[i].node_1->x,
				   Order_Array[i].node_1->y,
				   Order_Array[i].node_2->x,
				   Order_Array[i].node_2->y);
		}
	}

