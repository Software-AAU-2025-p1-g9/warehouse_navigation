#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "warehouse.h"

int node_pos(int size_x, int x, int y){
    return y * size_x + x;
}


int min(int n_1, int n_2) {
    return n_1 < n_2 ? n_1 : n_2;
}

int max(int n_1, int n_2) {
    return n_1 < n_2 ? n_2 : n_1;
}

float f_min(float n_1, float n_2) {
    return n_1 < n_2 ? n_1 : n_2;
}
void OrderRandomizer(int Order_Amount, order Order_Array[], node* Pickup[], int Pick_Length, node* Dropoff[], int Drop_Length, node* Shelves[], int Shelve_Length){

    unsigned int seed = (unsigned int)(time(NULL) ^ (uintptr_t)&seed); //mostly unpredictable seed for randomization
    srand(seed); //inputs the seed into the random function

    for(int i = 0; i < Order_Amount; i++)
    {
        if(rand() < RAND_MAX/2){ //tilfæld pickup. rand() chooses a number between 0 and RAND_MAX as such this generates a 50/50 chance for either pick or drop
            int Index_of_Pickup = rand() % Pick_Length; //find et tilfældigt tal mellem 0 og den største indeks af Pickup arrayet
            Order_Array[i].node_1 = Pickup[Index_of_Pickup]; //sætter order arrayets node_1 i [i] pladsen lige med et tilfældigt node fra pickuparrayet.

            int Index_of_Shelf =   rand() % Shelve_Length;  //det samme som ovenover bare med shelve arrayet og ikke pickup arrayet
            Order_Array[i].node_2 = Shelves[Index_of_Shelf]; // det samme som i pickup bare med node_2
        }

        else{ //tilfæld dropoff if not pickup then it must be dropoff
            int Index_of_Shelf = rand() % Shelve_Length; //det samme som shelve assigneren i tilfæld pickup
            Order_Array[i].node_1 = Shelves[Index_of_Shelf];

            int Index_of_Dropoff = rand() % Drop_Length; //det samme som de andre assigner, bare at den assigner fra Dropoff arrayet
            Order_Array[i].node_2 = Dropoff[Index_of_Dropoff];
        }

    }
}
