#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "warehouse.h"

void OrderRandomizer(int OrderAmount, order* OrderArray[], node* Pickup[], node* Dropoff[], node* Shelves[]);

int main(void) {
int OrderAmount;
printf("amount of orders");
scanf(" %d" , &OrderAmount);
order* orderarray[OrderAmount];

node* Shelves[99]; //MIDLERTIDIG VÆRDI FOR SIZE AF ARRAYET
node* Pickup[99]; //MIDLERTIDIG VÆRDI FOR SIZE AF ARRAYET
node* Dropoff[99]; //MIDLERTIDIG VÆRDI FOR SIZE AF ARRAYET




}
//find ud af om ARRAY size tingene tager hele længden eller kun der hvor der er variabler, fordi så skal de måske laves om

void OrderRandomizer(int OrderAmount, order* orderarray[], node* Pickup[], node* Dropoff[], node* Shelves[]){
 // anden ide til hvordan man kan lave random hylder orders lidt mere optimalt tror jeg
	srand(time(NULL));

for(int i = 0; i < OrderAmount; i++) //måske <= OrderAmount? skal lige teste først
{
	int pickordrop = random(1,2); //1 = pick up 2 = drop off. mening er de skal kunne ændres hver loop

	if(pickordrop == 1){ //tilfæld pickup

int indexofpickup = random(0, PickupArraySize);


	orderarray[i].node_1 = Pickup[indexofpickup]; //sætter node_1 lige med et tilfældigt node fra pickuparrayet. pickuparrayet burde have både x og y værdierne for alle pickup noder.

	int indexofshelve = random(0,hylderarraysize); //find et tilfældigt tal mellem 0 og den største indeks af hylder array


	//coden over antager at hylde arrayet består kun af node structs som hver peger på et enkelt shelve, hvor i dens helhed har den koordinatner for alle shelves.
	orderarray[i].node_2 = Shelves[indexofshelve];
	i++;
}

	if(pickordrop == 2){ //tilfæld dropoff

		int indexofshelve = random(0,hylderarraysize); //find et tilfældigt tal mellem 0 og den største indeks af hylder array

		orderarray[i]->node_1 = Shelves[indexofshelve];
		//coden over antager at hylde arrayet består kun af node structs som hver peger på et enkelt shelve, hvor i dens helhed har den koordinatner for alle shelves.


		//TESTING GROUND FOR RANDOM
		int DropoffLength = sizeof(Dropoff) / sizeof(Dropoff[0]);

		int indexofdropoff = rand() % (DropoffLength - 0 +1)+0; //finder random værdi mellem maximum Dropofflength og minimum 0
		// MAKE IT MORE RANDOM WITH PROBALY SRAND
		orderarray[i]->node_2 = Dropoff[indexofdropoff];

		i++;
	}
}

}







/*
if(pickordrop == 2) { //første ide til hvordan man kan lave random hylder orders

for(int i = 0; i < orderamount; i++)
{
test.node_1.x = random(0,maxsizex);
test.node_1.y = random(0,maxsizey);

for(int j; j <hylderarraysize; j++)	{
if(node_1 == hylderarray[j]){
//gem node_1 værdien indeni et array of order MANGLER
i++;
							}
else
j++;


									}
}

					}

}
*/