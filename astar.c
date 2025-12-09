#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "warehouse.h"


//Initializing functions
void calc_succeding(node** nodes, int x, int y, int map_id, int goal_x, int goal_y, int **open_nodes);
float h_calc(int x, int y, int goal_x, int goal_y);
void add_to_opennodes(int x, int y, int **open_nodes);
//Do not call create_open_nodes without freeing memory after
int** create_open_nodes(int total_area);



void astar(node** nodes, int start_x, int start_y, int goal_x, int goal_y, int map_id, int total_area) {
    int** open_nodes = create_open_nodes(total_area);

    //Calculating starting node
    nodes[start_y][start_x].g[map_id] = 0;
    nodes[start_y][start_x].h[map_id] = h_calc(start_x, start_y, goal_x, goal_y);

    //Tempeorary x and y used for calculations while still remembering originals
    //Setting these to the start node as this is the first node we calculate from
    int temp_x = start_x,
        temp_y = start_y;

    int step = 0; //Debug
    //Check all nodes with lowest f(n) until the open node with lowest f(n) is the goal node
    while (nodes[goal_y][goal_x].g[map_id] == INFINITY) {
        //Debug
        printf("\nCurrent node x%d y%d \n", temp_x, temp_y);
        step++;
        //Calculating succeding nodes from temp y & x
        calc_succeding(nodes, temp_x, temp_y, map_id, goal_x, goal_y, open_nodes);

        //Integer used in while loop for for-loop like function
        //lowestf keeps track of the lowest f(n) found
        int i = 0;
        float lowestf = INFINITY;

        int open_nodes_tracker = 0;
        //check open nodes for lowest f(n). Stop loop when the last open_node is found
        while (open_nodes[i][0] != INT_MAX) {
            //Setting the f(n) of the current i position
            float current_f = (nodes[open_nodes[i][0]][open_nodes[i][1]].g[map_id]+
                             nodes[open_nodes[i][0]][open_nodes[i][1]].h[map_id]);

            //Check if y is 0 or above as closed nodes get value of -1 to replace them.
            if (open_nodes[i][0] >= 0) {
                //Check if the current f is below the lowest found
                //If so set current x and y as lowest f
                if (current_f < lowestf) {
                    temp_y = open_nodes[i][0];
                    temp_x = open_nodes[i][1];
                    open_nodes_tracker = i;
                    lowestf = current_f;
                }
            }
            //Progress to next open node
            i++;
        }
        open_nodes[open_nodes_tracker][0] = -1;
        open_nodes[open_nodes_tracker][1] = -1;
    }

    //Debug
    printf("\nJeg nåede ud af while-loopet af en eller anden grund\n");
    printf("For mål er g = %f \n", nodes[goal_y][goal_x].g[map_id]);

    //Free memory from inner arrays
    for (int i = 0; i < total_area; i++) {
        free(open_nodes[i]);
    }
    //Free memory of the outer array
    free(open_nodes);
}

void calc_succeding(node** nodes, int x, int y, int map_id, int goal_x, int goal_y, int **open_nodes) {
    //For loop to check all succeding nodes (neighbour_count)
    for (int i = 0; i < nodes[y][x].neighbour_count; i++) {

        //Setting temporary int to the calculated new g for the succeding node
        float new_successor_g = nodes[y][x].successors[i]->cost + nodes[y][x].g[map_id];
        float old_successor_g = nodes[y][x].successors[i]->dest->g[map_id];

        //Checking if node is already calculated once and the earlier calculation had a higher g than now
        if (old_successor_g != INFINITY && old_successor_g > new_successor_g) {
            //Setting new g of node (no need for new h as this will be the same as last)
            nodes[y][x].successors[i]->dest->g[map_id] = new_successor_g;
            //Adding to open_nodes again

            add_to_opennodes(nodes[y][x].successors[i]->dest->x, nodes[y][x].successors[i]->dest->y, open_nodes);

        //Checking if node is not calculated. If so calculate it
        } else if (old_successor_g == INFINITY) {
            //Setting g and h of non-calculated node + adding to open nodes
            nodes[y][x].successors[i]->dest->g[map_id] = new_successor_g;
            nodes[y][x].successors[i]->dest->h[map_id] =
            h_calc(nodes[y][x].successors[i]->dest->x, nodes[y][x].successors[i]->dest->y, goal_x, goal_y);
            add_to_opennodes(nodes[y][x].successors[i]->dest->x, nodes[y][x].successors[i]->dest->y, open_nodes);
        }

    }

}

float h_calc(int x, int y, int goal_x, int goal_y) {
    //Checking if difference is bigger on the x or y axis to determine how to calculate
    /*Calculating difference of distance on x and y axis and adding this to the pyhtagoran
     *calculation with two times the smallest distance
     */
    if ((goal_x-x) > (goal_y-y)) {
        return (float)((goal_x-x)-(goal_y-y))+sqrtf((float)((goal_y-y)*(goal_y-y)*2));
    } else {
        return (float)((goal_y-y)-(goal_x-x))+sqrtf((float)((goal_x-x)*(goal_x-x)*2));
    }
}

void add_to_opennodes(int x, int y, int **open_nodes) {
    //initialising i used in later calculation
    int i = 0;
    while (open_nodes[i][0] != INT_MAX) {
        if (open_nodes[i][0] == y && open_nodes[i][1] == x) {
            open_nodes[i][0] = -1;
            open_nodes[i][1] = -1;
        }
        i++;
    }


    i = 0;
    //Setting i to the first open postition in the open_nodes array
    while (open_nodes[i][0] != INT_MAX && open_nodes[i][0] != -1) {
        i++;
    }

    //Setting the x and y of the open node
    open_nodes[i][0] = y;
    open_nodes[i][1] = x;
}

void reset_g(node** nodes, int x_size, int y_size, int map_id) {
    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            nodes[i][j].g[map_id] = INFINITY;
        }
    }
}

int** create_open_nodes(int total_area) {
    //Creating open nodes array of array
    int **open_nodes = malloc(total_area*sizeof(int*));
    if (!open_nodes) {
        printf("No more memory\n");
        exit(EXIT_FAILURE);
    }

    //Setting size of array and initializing array of arrays
    for (int i = 0; i < total_area; i++) {
        open_nodes[i] = malloc(2*sizeof(int));
        if (!open_nodes[i]) {
            printf("No more memory\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < 2; j++) {
            open_nodes[i][j] = INT_MAX;
        }

    }
    return open_nodes;
}