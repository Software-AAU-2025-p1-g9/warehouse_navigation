#include <math.h>
#include "warehouse.h"

//placeholder
int optimalpath[OPENNODESAMOUNT][2];


void astar(node** nodes, int start_x, int start_y, int goal_x, int goal_y, int map_id, int* open_nodes, int* optimal_path);
void calcsucceding(node** nodes, int x, int y, int map_id, int goalx, int goaly);
int hcalc(int x, int y, int goalx, int goaly);
void addToOpennodes(int x, int y);
void calcastar(node** nodes, int startx, int starty, int goalx, int goaly, int map_id);

void astar(node** nodes, int startx, int starty, int goalx, int goaly, int map_id, int* open_nodes, int* optimal_path) {

    //Calculating starting node
    nodes[starty][startx].g[map_id] = 0;
    nodes[starty][startx].h[map_id] = hcalc(startx, starty, goalx, goaly);

    int temp_x = startx,
        temp_y = starty;

    //Check all nodes with lowest f(n) until the open node with lowest f(n) is the goal node
    while (nodes[goaly][goalx].g[map_id] == INFINITY) {
        calcsucceding(nodes, temp_x, temp_y, map_id, goalx, goaly);

        int temp = 0,
            i = 0,
            lowestf = INFINITY;

        //check open nodes for lowest f(n)
        while (opennodes[i][0] != INFINITY) {
            int current_f = (nodes[opennodes[i][0]][opennodes[i][1]].g[map_id]+
                             nodes[opennodes[i][0]][opennodes[i][1]].h[map_id]);

            if (opennodes[i][0] >= 0) {
                //Check if y is 0 or above as closed nodes get value of -1 to replace them.
                if (current_f < lowestf) {
                    temp_y = opennodes[i][0];
                    temp_x = opennodes[i][1];
                    lowestf = current_f;
                }
            }
            i++;
        }
    }

    calcastar(nodes, startx, starty, goalx, goaly, map_id);

}

void calcsucceding(node** nodes, int x, int y, int map_id, int goalx, int goaly) {
    //Temporary int
    int temp = 0;

    for (int i = 0; i < nodes[starty][startx].neighbor_count; i++) {
        //Setting temporary int to the calculated new g for the succeding node
        temp = (nodes[y][x].successors[i].cost + nodes[y][x].g[map_id]);

        //Checking if node is already calculated once
        if (nodes[y][x].successors[i].dest.g[map_id] != INFINITY && nodes[y][x].successors[i].dest.g[map_id] > temp) {

                nodes[y][x].successors[i].dest.g[map_id] = temp;
                nodes[y][x].successors[i].dest.h[map_id] =
                hcalc(nodes[y][x].successors[i].dest.x, nodes[y][x].successors[i].dest.y, goalx, goaly);
                addToOpennodes(x,y);

        } else {
            nodes[y][x].successors[i].dest.g[map_id] = temp;
            nodes[y][x].successors[i].dest.h[map_id] =
            hcalc(nodes[y][x].successors[i].dest.x, nodes[y][x].successors[i].dest.y, goalx, goaly);
            addToOpennodes(x,y);
        }

    }

}

int hcalc(int x, int y, int goalx, int goaly) {
    if ((goalx-x) > (goaly-y) {
        return ((goalx-x-goaly+y)+sqrt((goaly-y)*(goaly-y)*2)
    } else {
        return ((goaly-y-goalx+x)+sqrt((goalx-x)*(goalx-x)*2)
    }
}

void addToOpennodes(int x, int y) {

        i = 0;

    while (opennodes[i][0] != INT_MAX || opennodes[i][0] != -1) {
        i++;
    }
    opennodes[i][0] = y;
    opennodes[i][1] = x;
}

/*
void calcastar(node** nodes, int startx, int starty, int goalx, int goaly, int map_id) {
    //helping int
    int x = goalx,
        y = goaly,
        numberstep = 0;

    optimalpath[0][0] = y;
    optimalpath[0][1] = x;

    numberstep = 1;

    while (x != startx && y != starty) {
        //Temporary int
        int temp= INFINITY;

        int temp_x = x,
            temp_y = y;

        for (int i = 0; i < 8; i++) {
            if (nodes[temp_y][temp_x].successors[i].dest.g[map_id] < n) {
                temp = nodes[temp_y][temp_x].successors[i].dest.g[map_id];
                x = nodes[temp_y][temp_x].successors[i].dest.x;
                y = nodes[temp_y][temp_x].successors[i].dest.y;
            }
        }

        optimalpath[numberstep][0] = y;
        optimalpath[numberstep][1] = x;

        numberstep++;
    }
}
*/