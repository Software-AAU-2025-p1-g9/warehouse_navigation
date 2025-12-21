#include <float.h>
#include <stdio.h>
#include "warehouse.h"
#include <stdlib.h>
#include <limits.h>


void path_printer(node** nodes, int goal_x, int goal_y, int start_x, int start_y, int map_id) {
    int x = goal_x,
        y = goal_y;

    int array_lenght = (abs(goal_x-start_x)+abs(goal_y-start_y))*2;

    int shortest_path[array_lenght][2];

    for (int i = 0; i < array_lenght; i++) {
        shortest_path[i][0] = INT_MAX;
        shortest_path[i][1] = INT_MAX;
    }

    int array_position = 0;

    shortest_path[array_position][0] = goal_y;
    shortest_path[array_position][1] = goal_x;
    array_position = 1;

    printf("\nThe shortest path between x%dy%d nad x%dy%d is: \n", start_x, start_y, goal_x, goal_y);

    while ((x != start_x) || (y != start_y)) {
        //Setting temporary int to the keep track of lowest g
        float lowest_g = INFINITY;

        int temp_x = x,
            temp_y = y;



        //For loop to find neighbour with lowest g + cost
        for (int i = 0; i < nodes[y][x].neighbour_count; i++) {
            //Checking if node is already calculated once and the earlier calculation had a higher g than now
            float current_g = nodes[y][x].successors[i]->dest->g[map_id]+nodes[y][x].successors[i]->cost;

            if (lowest_g > current_g) {
                lowest_g = current_g;
                temp_x = nodes[y][x].successors[i]->dest->x;
                temp_y = nodes[y][x].successors[i]->dest->y;
            } else if ((start_x == nodes[y][x].successors[i]->dest->x) && (start_y == nodes[y][x].successors[i]->dest->y)) {
                lowest_g = 0;
                temp_x = nodes[y][x].successors[i]->dest->x;
                temp_y = nodes[y][x].successors[i]->dest->y;
            }

        }
        x = temp_x;
        y = temp_y;

        shortest_path[array_position][0] = y;
        shortest_path[array_position][1] = x;
        array_position++;

    }

    for (int i = array_position-1; i > 0; i--) {
        printf("x%d y%d \n", shortest_path[i][1], shortest_path[i][0]);
    }
    printf("x%d y%d \n \n", shortest_path[0][1], shortest_path[0][0]);



}