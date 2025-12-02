#include <math.h>
#include "warehouse.h"

int OPENNODESAMOUNT = 100;
int opennodes[OPENNODESAMOUNT][2];

//placeholder
int optimalpath[OPENNODESAMOUNT][2];


void astar(node** nodes, int startx, int starty, int goalx, int goaly, int x_size);
void calcsucceding(node** nodes, int x, int y, int localgh, int goalx, int goaly);
int hcalc(int x, int y, int goalx, int goaly);
void addToOpennodes(int x, int y);
void calcastar(node** nodes, int startx, int starty, int goalx, int goaly, int localgh);

void astar(node** nodes, int startx, int starty, int goalx, int goaly, int x_size) {
    //placeholder
    for (int i = 0; i < OPENNODESAMOUNT; i++) {
        optimalpath[i][0] = INFINITY;
        optimalpath[i][1] = INFINITY;
    }

    for (int i = 0; i < OPENNODESAMOUNT; i++) {
        opennodes[i][0] = INFINITY;
        opennodes[i][1] = INFINITY;
    }

    int localgh = node_pos(x_size, startx, starty);

    //Calculating starting node
    nodes[starty][startx].g[localgh] = 0;
    nodes[starty][startx].h[localgh] = hcalc(startx, starty, goalx, goaly);

    int nx = startx,
        ny = starty;

    //Check all nodes with lowest f(n) until the open node with lowest f(n) is the goal node
    while (nodes[goaly][goalx].g[localgh] == INFINITY) {
        calcsucceding(nodes, nx, ny, localgh, goalx, goaly);

        int n = 0,
            i = 0,
            lowestf = INFINITY;

        //check open nodes for lowest f(n)
        while (n == 0) {
            //If y value is infinity we have reached the end of open nodes
            if (opennodes[i][0] == INFINITY) {
                n = 1;
            } else if (opennodes[i][0] >= 0) {
                //Check if y is 0 or above as closed nodes get value of -1 to replace them.
                if ((nodes[opennodes[i][0]][opennodes[i][1]].g[localgh]+
                     nodes[opennodes[i][0]][opennodes[i][1]].h[localgh]) < lowestf) {
                    ny = opennodes[i][0];
                    nx = opennodes[i][1];
                    lowestf = (nodes[opennodes[i][0]][opennodes[i][1]].g[localgh]+
                               nodes[opennodes[i][0]][opennodes[i][1]].h[localgh]);
                }
            }
            i++;
        }
    }

    calcastar(nodes, startx, starty, goalx, goaly, localgh);

}

void calcsucceding(node** nodes, int x, int y, int localgh, int goalx, int goaly) {
    //Helper int
    int n = 0;

    for (int i = 0; i < 8; i++) {
        //Setting helper int to the calculated new g for the succeding node
        n = (nodes[y][x].successors[i].cost + nodes[y][x].g[localgh]);

        //Checking if node is already calculated once
        if (nodes[y][x].successors[i].dest.g[localgh] != INFINITY) {
            //If node is already calcualed check if new calculated g is lower than old
            if (nodes[y][x].successors[i].dest.g[localgh] > n) {
                nodes[y][x].successors[i].dest.g[localgh] = n;
                nodes[y][x].successors[i].dest.h[localgh] =
                hcalc(nodes[y][x].successors[i].dest.x, nodes[y][x].successors[i].dest.y, goalx, goaly);
                addToOpennodes(x,y);
            }
        } else {
            nodes[y][x].successors[i].dest.g[localgh] = n;
            nodes[y][x].successors[i].dest.h[localgh] =
            hcalc(nodes[y][x].successors[i].dest.x, nodes[y][x].successors[i].dest.y, goalx, goaly);
            addToOpennodes(x,y);
        }

    }

}

int hcalc(int x, int y, int goalx, int goaly) {
    return sqrt(((goalx-x)*(goalx-x))+((goaly-y)*(goaly-y)));
}

void addToOpennodes(int x, int y) {
    int n = 0,
        i = 0;

    while (n == 0) {

        if (opennodes[i][0] != INFINITY) {
            y = opennodes[i][0];
            x = opennodes[i][1];
            n++;
        }
        i++;
    }
}

void calcastar(node** nodes, int startx, int starty, int goalx, int goaly, int localgh) {
    //helping int
    int x = goalx,
        y = goaly,
        numberstep = 0;

    optimalpath[0][0] = y;
    optimalpath[0][1] = x;

    numberstep = 1;

    while (x != startx && y != starty) {
        //Helper int
        int n = INFINITY;

        int nx = x,
            ny = y;

        for (int i = 0; i < 8; i++) {
            if (nodes[ny][nx].successors[i].dest.g[localgh] < n) {
                n = nodes[ny][nx].successors[i].dest.g[localgh];
                x = nodes[ny][nx].successors[i].dest.x;
                y = nodes[ny][nx].successors[i].dest.y;
            }
        }

        optimalpath[numberstep][0] = y;
        optimalpath[numberstep][1] = x;

        numberstep++;
    }
}