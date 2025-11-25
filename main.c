#include "warehouse.h"

int orderrandomizer(order test, int orderamount);

int main(void) {
int orderamount;
printf("amount of orders");
scanf(" %s" , &orderamount);

}


int orderrandomizer(order test, int orderamount){
int pickordrop = random(1,2) //1 = pick up 2 = drop off

if(pickordrop == 1){ // anden ide til hvordan man kan lave random hylder orders lidt mere optimalt tror jeg

for(int i = 0; i < orderamount; i++)
{
int indexofshelve = random(0,hylderarraysize) //find et tilfældigt tal mellem 0 og den største indeks af hylder array

test.node_1 = hylderarray[indexofshelve] //sætter node_1 lige med et tilfældigt node fra hyldearrayet. hyldearrayet burde have både x og y værdierne for alle hylde noder.

//gem node_1 værdien indeni et array of order MANGLER
i++;
}
}


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