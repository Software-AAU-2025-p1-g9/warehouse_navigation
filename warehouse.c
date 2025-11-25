#include "warehouse.h"

int node_pos(int size_x, int x, int y){
    return y * size_x + x;
}

double min(double n_1, double n_2) {
    return n_1 < n_2 ? n_1 : n_2;
}