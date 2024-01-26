#ifndef SEARCH_H
#define SEARCH_H

#include <vector>

class Board;

int search(Board &b, int depth);
int negamax(Board &b, int depth, int alpha, int beta, int &nodes);
int quiescence_search(Board &b, int alpha, int beta, int &nodes);
void order_moves(std::vector<int> &moves);

#endif