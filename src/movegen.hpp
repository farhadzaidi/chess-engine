#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>

class Board;

// Generates psuedo-legal moves (i.e. does not enforce checks)

std::vector<int> get_pawn_moves(Board &b, int sq);
std::vector<int> get_piece_moves(Board &b, int sq, int directions[8], int slide);

#endif 