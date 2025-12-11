//
// Created by Alex on 11-12-2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "warehouse.h"
#include "order_generator.h"

void order_randomizer(int order_amount, order order_array[], node* pickup[], int pick_length, node* dropoff[], int drop_length, node* shelves[], int shelves_length){
    for(int i = 0; i < order_amount; i++)
    {
        if(rand() < RAND_MAX/2){ //tilfæld pickup. rand() chooses a number between 0 and RAND_MAX as such this generates a 50/50 chance for either pick or drop
            int index_of_pickup = rand() % pick_length; //find et tilfældigt tal mellem 0 og den største indeks af Pickup arrayet
            order_array[i].node_1 = pickup[index_of_pickup]; //sætter order arrayets node_1 i [i] pladsen lige med et tilfældigt node fra pickuparrayet.

            int index_of_shelf =   rand() % shelves_length;  //det samme som ovenover bare med shelve arrayet og ikke pickup arrayet
            order_array[i].node_2 = shelves[index_of_shelf]; // det samme som i pickup bare med node_2
        }

        else{ //tilfæld dropoff if not pickup then it must be dropoff
            int index_of_shelf = rand() % shelves_length; //det samme som shelve assigneren i tilfæld pickup
            order_array[i].node_1 = shelves[index_of_shelf];

            int index_of_dropoff = rand() % drop_length; //det samme som de andre assigner, bare at den assigner fra Dropoff arrayet
            order_array[i].node_2 = dropoff[index_of_dropoff];
        }

    }
}