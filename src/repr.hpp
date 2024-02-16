#ifndef REPR_H
#define REPR_H

#include <string>

class Board;

void load_from_FEN(Board &b, std::string FEN);
void print_board(Board &b);
void print_attr(Board &b);

#endif