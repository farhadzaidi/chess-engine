#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>

class Board;

// Generates psuedo-legal moves (i.e. does not enforce checks)

std::vector<int> gen_moves(Board &b);
std::vector<int> validate_moves(Board &b, std::vector<int> moves);
void get_pawn_moves(Board &b, int sq, std::vector<int> &moves);
void get_piece_moves(Board &b, int sq, const int directions[8], int slide,
	std::vector<int> &moves);

#endif 