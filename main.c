#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "warehouse.h"

void OrderRandomizer(int OrderAmount, order* OrderArray[], node* Pickup[], int PickLength, node* Dropoff[], int DropLength, node* Shelves[], int ShelveLength);

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

	int OrderAmount;
	printf("amount of orders");
	scanf(" %d" , &OrderAmount);
	order* OrderArray[OrderAmount];
	OrderRandomizer(OrderAmount, OrderArray, Pickup, PickLength,Dropoff, DropLength, Shelves, ShelveLength);

	// slet for loop i endeligt version nedenunder kun for test.
	for (int i = 0; i < OrderAmount; i++) {
		printf("Order %d: node_1 = (%d,%d), node_2 = (%d,%d)\n",
				i+1,
			   OrderArray[i]->node_1->x,
			   OrderArray[i]->node_1->y,
			   OrderArray[i]->node_2->x,
			   OrderArray[i]->node_2->y);
	}
}

void OrderRandomizer(int OrderAmount, order* OrderArray[], node* Pickup[], int PickLength, node* Dropoff[], int DropLength, node* Shelves[], int ShelveLength){

	unsigned int seed = (unsigned int)(time(NULL) ^ (uintptr_t)&seed); //mostly unpredictable seed for randomization
	srand(seed); //inputs the seed into the random function

for(int i = 0; i < OrderAmount; i++)
{
	OrderArray[i] = malloc(sizeof(order)); //allocation of space for the array.
	if (OrderArray[i] == NULL) {
		printf("Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	int rnd = rand();
	int pickordrop = (rnd < RAND_MAX/2) ? 1 : 2; //1 = pick up 2 = drop off. mening er de skal kunne ændres hver loop
  //rand chooses a number between 0 and RAND_MAX as such this generates a 50/50 chance for either pick or drop

if(pickordrop == 1){ //tilfæld pickup
	int indexofpickup = rand() % PickLength; //åbenbart er der et bias når man laver modulo af rand(), hvor mindre number dukker op mere ofte(måske find fix?)
	OrderArray[i]->node_1 = Pickup[indexofpickup]; //sætter node_1 lige med et tilfældigt node fra pickuparrayet. pickuparrayet burde have både x og y værdierne for alle pickup noder.

	int indexofshelve = rand() % ShelveLength; //find et tilfældigt tal mellem 0 og den største indeks af hylder array
	//coden over antager at hylde arrayet består kun af node structs som hver peger på et enkelt shelve, hvor i dens helhed har den koordinatner for alle shelves.
	OrderArray[i]->node_2 = Shelves[indexofshelve];
}

if(pickordrop == 2){ //tilfæld dropoff
	int indexofshelve = rand() % ShelveLength; //det samme som shelve assigneren i tilfæld pickup
	OrderArray[i]->node_1 = Shelves[indexofshelve];

	int indexofdropoff = rand() % DropLength; //det samme som de andre assigner, bare at den assigner fra Dropoff arrayet
	OrderArray[i]->node_2 = Dropoff[indexofdropoff];
}

}

}