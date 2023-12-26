#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>

class Board;

std::vector<int> get_pawn_moves(Board &b, int sq);
std::vector<int> get_sliding_moves(Board &b, int sq, int directions);

#endif 