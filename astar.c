#include <math.h>
#include <stdlib.h>
#include <limits.h>

#include "warehouse.h"

//placeholder

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

    int temp_x = start_x,
        temp_y = start_y;

    //Check all nodes with lowest f(n) until the open node with lowest f(n) is the goal node
    while (nodes[goal_y][goal_x].g[map_id] == INFINITY) {
        calcsucceding(nodes, temp_x, temp_y, map_id, goal_x, goal_y, open_nodes);

        int i = 0,
            lowestf = INFINITY;

        //check open nodes for lowest f(n)
        while (open_nodes[i][0] != INFINITY) {
            int current_f = (nodes[open_nodes[i][0]][open_nodes[i][1]].g[map_id]+
                             nodes[open_nodes[i][0]][open_nodes[i][1]].h[map_id]);

            if (open_nodes[i][0] >= 0) {
                //Check if y is 0 or above as closed nodes get value of -1 to replace them.
                if (current_f < lowestf) {
                    temp_y = open_nodes[i][0];
                    temp_x = open_nodes[i][1];
                    lowestf = current_f;
                }
            }
            i++;
        }
    }
    free(open_nodes);
}

void calcsucceding(node** nodes, int x, int y, int map_id, int goal_x, int goal_y, int **open_nodes) {
    //Temporary int
    int temp = 0;

    for (int i = 0; i < nodes[y][x].neighbour_count; i++) {
        //Setting temporary int to the calculated new g for the succeding node
        temp = (nodes[y][x].successors[i]->cost + nodes[y][x].g[map_id]);

        //Checking if node is already calculated once
        if (nodes[y][x].successors[i]->dest->g[map_id] != INFINITY && nodes[y][x].successors[i]->dest->g[map_id] > temp) {

                nodes[y][x].successors[i]->dest->g[map_id] = temp;
                nodes[y][x].successors[i]->dest->h[map_id] =
                hcalc(nodes[y][x].successors[i]->dest->x, nodes[y][x].successors[i]->dest->y, goal_x, goal_y);
                addToOpennodes(x,y, open_nodes);

        } else {
            nodes[y][x].successors[i]->dest->g[map_id] = temp;
            nodes[y][x].successors[i]->dest->h[map_id] =
            hcalc(nodes[y][x].successors[i]->dest->x, nodes[y][x].successors[i]->dest->y, goal_x, goal_y);
            addToOpennodes(x, y, open_nodes);
        }

    }

}

int hcalc(int x, int y, int goal_x, int goal_y) {
    if ((goal_x-x) > (goal_y-y)) {
        return ((goal_x-x-goal_y+y)+sqrt((goal_y-y)*(goal_y-y)*2));
    } else {
        return ((goal_y-y-goal_x+x)+sqrt((goal_x-x)*(goal_x-x)*2));
    }
}

void addToOpennodes(int x, int y, int **open_nodes) {

    int i = 0;

    while (open_nodes[i][0] != INT_MAX || open_nodes[i][0] != -1) {
        i++;
    }
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
    int **open_nodes = malloc(total_area*sizeof(int));

    for (int i = 0; i < total_area; i++) {
        open_nodes[i] = malloc(total_area*sizeof(int));
    }

    for (int i = 0; i < total_area; i++) {
        for (int j = 0; j < total_area; j++) {
            open_nodes[i][j] = INT_MAX;
        }

    }
}