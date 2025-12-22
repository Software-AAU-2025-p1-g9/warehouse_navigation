#include "warehouse.h"

int node_pos(int size_x, int x, int y){
    return y * size_x + x;
}


int min(int n_1, int n_2) {
    return n_1 < n_2 ? n_1 : n_2;
}

int max(int n_1, int n_2) {
    return n_1 < n_2 ? n_2 : n_1;
}

float f_min(float n_1, float n_2) {
    return n_1 < n_2 ? n_1 : n_2;
}

int f_eq(float n_1, float n_2) {
    return n_1 - 0.001 < n_2 && n_1 + 0.001 > n_2;
}