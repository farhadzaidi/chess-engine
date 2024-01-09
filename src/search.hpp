#ifndef ENGINE_H
#define ENGINE_H

class Board;

struct Move_Eval {
	int move;
	int eval;

	Move_Eval(int move, int eval) : move(move), eval(eval) {};
};

int random_move(Board &b);
Move_Eval minimax(Board &b, int depth, int alpha, int beta);

#endif