#include <float.h>
#include <stdio.h>
#include "warehouse.h"
#include <limits.h>


void path_printer(node** nodes, int goal_x, int goal_y, int start_x, int start_y, int map_id) {
    int x = goal_x,
        y = goal_y;

    printf("\nThe shortest path between x%dy%d nad x%dy%d is: \n", goal_x, goal_y, start_x, start_y);

    while ((x != start_x) || (y != start_y)) {
        //Printing current step
        printf("x%d y%d \n", x, y);

        //Setting temporary int to the keep track of lowest g
        float lowest_g = INFINITY;

        int temp_x = x,
            temp_y = x;

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
    }

    printf("x%d y%d \n \n", x, y);

}