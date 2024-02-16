#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>

class Board;

// Generates psuedo-legal moves (i.e. does not enforce checks)
std::vector<int> gen_moves(Board &b);

// Returns a vector of legal moves given a vector of psuedo-legal moves
std::vector<int> validate_moves(Board &b, std::vector<int> moves);

#endif 