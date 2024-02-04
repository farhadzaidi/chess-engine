#ifndef SEARCH_H
#define SEARCH_H

#include <vector>

class Board;

const int MAX_NODES = 250000;

int iterative_search(Board &b);
int search(Board &b, int depth, int prev_best_move, int &search_cancelled);
int negamax(Board &b, int depth, int alpha, int beta, int &nodes, int &search_cancelled);
int quiescence_search(Board &b, int alpha, int beta, int &nodes, int &search_cancelled);
void order_moves(std::vector<int> &moves, int prev_best_move = 0);

#endif