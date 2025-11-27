#include "warehouse.h"

int orderrandomizer(order test, int orderamount, order[] first);

int main(void) {
int orderamount;
printf("amount of orders");
scanf(" %s" , &orderamount);
order[orderamount] orderarray;



}


int orderrandomizer(order test, int orderamount, order[] orderarray){
 // anden ide til hvordan man kan lave random hylder orders lidt mere optimalt tror jeg

for(int i = 0; i < orderamount; i++) //måske <= orderamount? skal lige teste først
{
	int pickordrop = random(1,2) //1 = pick up 2 = drop off. mening er de skal kunne ændres hver loop

	if(pickordrop == 1){ //tilfæld pickup

int indexofpickup = random(0, PickupArraySize)

test.node_1 = pickuparray(indexofpickup)
orderarray[i].node_1 = test.node_1;

	int indexofshelve = random(0,hylderarraysize) //find et tilfældigt tal mellem 0 og den største indeks af hylder array

	test.node_2 = hylderarray[indexofshelve] //sætter node_1 lige med et tilfældigt node fra hyldearrayet. hyldearrayet burde have både x og y værdierne for alle hylde noder.
	//coden over antager at hylde arrayet består kun af node structs som hver peger på et enkelt shelve, hvor i dens helhed har den koordinatner for alle shelves.
	orderarray[i].node_2 = test.node_2;
	i++;
}

	if(pickordrop == 2){ //tilfæld dropoff

		int indexofshelve = random(0,hylderarraysize) //find et tilfældigt tal mellem 0 og den største indeks af hylder array

			test.node_1 = hylderarray[indexofshelve] //sætter node_1 lige med et tilfældigt node fra hyldearrayet. hyldearrayet burde have både x og y værdierne for alle hylde noder.
			//coden over antager at hylde arrayet består kun af node structs som hver peger på et enkelt shelve, hvor i dens helhed har den koordinatner for alle shelves.
			orderarray[i].node_1 = test.node_1;

		int indexofdropoff = random(0, DropoffArraySize)

		test.node_2 = DropoffArray(indexofdropoff)
		orderarray[i].node_2 = test.node_2;

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