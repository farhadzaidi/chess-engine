#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>

class Board;

std::vector<int> pawn_moves(Board &b, int sq);
std::vector<int> get_piece_moves(Board &b, int sq, int directions, int sliding);

#endif 