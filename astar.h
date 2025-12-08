#include "warehouse.h"

#ifndef ASTAR_H
#define ASTAR_H

void astar(node** nodes, int start_x, int start_y, int goal_x, int goal_y, int map_id, int total_area);
void resetg(node** nodes, int x_size, int y_size, int map_id);
#endif //ASTAR_H.h"