#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

class Board;

int search(Board &b, int depth);
int negamax(Board &b, int depth, int alpha, int beta);
void order_moves(std::vector<int> &moves);

#endif