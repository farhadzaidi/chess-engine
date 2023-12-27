#ifndef REPR_H
#define REPR_H

#include <string>

class Board;

int chess_to_sq(char rank, char file);
int chess_to_sq(std::string chess);
std::string sq_to_chess(int sq);
void load_from_FEN(Board &b, std::string FEN);
void print_board(Board &b);
void print_attr(Board &b);

#endif 