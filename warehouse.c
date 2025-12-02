#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "warehouse.h"

int node_pos(int size_x, int x, int y){
    return y * size_x + x;
}

void OrderRandomizer(int OrderAmount, order* OrderArray[], node* Pickup[], int PickLength, node* Dropoff[], int DropLength, node* Shelves[], int ShelveLength){

    unsigned int seed = (unsigned int)(time(NULL) ^ (uintptr_t)&seed); //mostly unpredictable seed for randomization
    srand(seed); //inputs the seed into the random function

    for(int i = 0; i < OrderAmount; i++)
    {
        OrderArray[i] = malloc(sizeof(order)); //allocation of space for the order array.
        if (OrderArray[i] == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        int rnd = rand();
        int pickordrop = (rnd < RAND_MAX/2) ? 1 : 2; //1 = pick up 2 = drop off. Nyt tal vælges hver loop
        //rand chooses a number between 0 and RAND_MAX as such this generates a 50/50 chance for either pick or drop

        if(pickordrop == 1){ //tilfæld pickup
            int indexofpickup = UnbiasedRand(PickLength); //find et tilfældigt tal mellem 0 og den største indeks af Pickup arrayet
            OrderArray[i]->node_1 = Pickup[indexofpickup]; //sætter order arrayets node_1 i [i] pladsen lige med et tilfældigt node fra pickuparrayet.

            int indexofshelve =  UnbiasedRand(ShelveLength);  //det samme som ovenover bare med shelve arrayet og ikke pickup arrayet
            OrderArray[i]->node_2 = Shelves[indexofshelve]; // det samme som i pickup bare med node_2
        }

        if(pickordrop == 2){ //tilfæld dropoff
            int indexofshelve = UnbiasedRand(ShelveLength); //det samme som shelve assigneren i tilfæld pickup
            OrderArray[i]->node_1 = Shelves[indexofshelve];

            int indexofdropoff = UnbiasedRand(DropLength); //det samme som de andre assigner, bare at den assigner fra Dropoff arrayet
            OrderArray[i]->node_2 = Dropoff[indexofdropoff];
        }

    }
}

    int UnbiasedRand(int MaxValueforRandWanted) { //funktion som 'fixer' rand bias(f.eks. rand % 3 har fire resultater = 0,1,2,0 altså der er dobbelt chance for 0. Dette fixer funktionen)
    int r, limit = RAND_MAX - RAND_MAX % MaxValueforRandWanted; //finder den højeste acceptabel værdi, som ikke skaber bias
    do { r = rand(); } while (r >= limit); //find et random værdi og hvis rand er over limit(skaber bias) kør igen indtil vi er under limit
    return r % MaxValueforRandWanted; //returnere tilfældig index number for et array(hvis den indsætte værdi er arrayets length)
    }