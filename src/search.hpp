#ifndef ENGINE_H
#define ENGINE_H

class Board;

int search(Board &b, int depth);
int negamax(Board &b, int depth, int alpha, int beta);

#endif