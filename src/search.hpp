#ifndef ENGINE_H
#define ENGINE_H

#include <utility>

class Board;

int random_move(Board &b);
std::pair<int, int> minimax(Board &b, int depth);

#endif