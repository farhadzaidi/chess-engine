#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

class Board;

int search(Board &b, int depth);
int negamax(Board &b, int depth, int alpha, int beta, int &nodes);
void order_moves(std::vector<int> &moves);

// Transposition table entries include the following information:
	// Depth at which the position was found (4 bits)
	// Evaluation of the position (14 bits)

#endif