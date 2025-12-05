#include <math.h>
#include <stdlib.h>
#include <limits.h>

#include "warehouse.h"


//Initializing functions
void astar(node** nodes, int start_x, int start_y, int goal_x, int goal_y, int map_id, int total_area, int **open_nodes);
void calcsucceding(node** nodes, int x, int y, int map_id, int goal_x, int goal_y, int **open_nodes);
int hcalc(int x, int y, int goal_x, int goal_y);
void addToOpennodes(int x, int y, int **open_nodes);
void resetg(node** nodes, int x_size, int y_size, int map_id);
//Do not call createOpennodes without freeing memory after
void createOpennodes(int total_area);



void astar(node** nodes, int start_x, int start_y, int goal_x, int goal_y, int map_id, int total_area, int **open_nodes) {
    createOpennodes(total_area);

    //Calculating starting node
    nodes[start_y][start_x].g[map_id] = 0;
    nodes[start_y][start_x].h[map_id] = hcalc(start_x, start_y, goal_x, goal_y);

    //Tempeorary x and y used for calculations while still remembering originals
    //Setting these to the start node as this is the first node we calculate from
    int temp_x = start_x,
        temp_y = start_y;

    //Check all nodes with lowest f(n) until the open node with lowest f(n) is the goal node
    while (nodes[goal_y][goal_x].g[map_id] == INFINITY) {
        //Calculating succeding nodes from temp y & x
        calcsucceding(nodes, temp_x, temp_y, map_id, goal_x, goal_y, open_nodes);

        //Integer used in while loop for for loop like function
        //lowestf keeps track of the lowest f(n) found
        int i       = 0,
            lowestf = INFINITY;

        //check open nodes for lowest f(n). Stop loop when the last open_node is found
        while (open_nodes[i][0] != INFINITY) {
            //Setting the f(n) of the current i position
            int current_f = (nodes[open_nodes[i][0]][open_nodes[i][1]].g[map_id]+
                             nodes[open_nodes[i][0]][open_nodes[i][1]].h[map_id]);

            //Check if y is 0 or above as closed nodes get value of -1 to replace them.
            if (open_nodes[i][0] >= 0) {
                //Check if the current f is below the lowest found
                //If so set current x and y as lowest f
                if (current_f < lowestf) {
                    temp_y = open_nodes[i][0];
                    temp_x = open_nodes[i][1];
                    lowestf = current_f;
                }
            }
            //Progress to next open node
            i++;
        }
    }
    //Free memory from createOpennodes function
    free(open_nodes);
}

void calcsucceding(node** nodes, int x, int y, int map_id, int goal_x, int goal_y, int **open_nodes) {
    //Temporary integer
    int temp = 0;

    //For loop to check all succeding nodes (neighbour_count)
    for (int i = 0; i < nodes[y][x].neighbour_count; i++) {
        //Setting temporary int to the calculated new g for the succeding node
        temp = (nodes[y][x].successors[i]->cost + nodes[y][x].g[map_id]);

        //Checking if node is already calculated once and the earlier calculation had a higher g than now
        if (nodes[y][x].successors[i]->dest->g[map_id] != INFINITY && nodes[y][x].successors[i]->dest->g[map_id] > temp) {
            //Setting new g of node (no need for new h as this will be the same as last)
            nodes[y][x].successors[i]->dest->g[map_id] = temp;
            //Adding to open_nodes again
            addToOpennodes(x,y, open_nodes);

        //Checking if node is not calculated. If so calculate it
        } else if (nodes[y][x].successors[i]->dest->g[map_id] == INFINITY) {
            //Setting g and h of non-calculated node + adding to open nodes
            nodes[y][x].successors[i]->dest->g[map_id] = temp;
            nodes[y][x].successors[i]->dest->h[map_id] =
            hcalc(nodes[y][x].successors[i]->dest->x, nodes[y][x].successors[i]->dest->y, goal_x, goal_y);
            addToOpennodes(x, y, open_nodes);
        }

    }

}

int hcalc(int x, int y, int goal_x, int goal_y) {
    //Checking if difference is bigger on the x or y axis to determine how to calculate
    /*Calculating difference of distance on x and y axis and adding this to the pyhtagoran
     *calculation with two times the smallest distance
     */
    if ((goal_x-x) > (goal_y-y)) {
        return ((goal_x-x-goal_y+y)+sqrt((goal_y-y)*(goal_y-y)*2));
    } else {
        return ((goal_y-y-goal_x+x)+sqrt((goal_x-x)*(goal_x-x)*2));
    }
}

void addToOpennodes(int x, int y, int **open_nodes) {
    //initialising i used in later calculation
    int i = 0;

    //Setting i to the first open postition in the open_nodes array
    while (open_nodes[i][0] != INT_MAX || open_nodes[i][0] != -1) {
        i++;
    }

    //Setting the x and y of the open node
    open_nodes[i][0] = y;
    open_nodes[i][1] = x;
}

void resetg(node** nodes, int x_size, int y_size, int map_id) {
    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            nodes[i][j].g[map_id] = INFINITY;
        }
    }
}

void createOpennodes(int total_area) {
    //Creating open nodes array of array
    int **open_nodes = malloc(total_area*sizeof(int));

    //Setting size of arrays in array
    for (int i = 0; i < total_area; i++) {
        open_nodes[i] = malloc(total_area*sizeof(int));
    }

    //Initializing array of arrays
    for (int i = 0; i < total_area; i++) {
        for (int j = 0; j < total_area; j++) {
            open_nodes[i][j] = INT_MAX;
        }

    }
}