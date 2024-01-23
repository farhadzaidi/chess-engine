#include "constants.hpp"
#include "search.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"

#include <iostream>

int search(Board &b, int depth) {
	int alpha = -INF;
	int beta = INF;
	int best_move = 0;
	int nodes = 0;
	std::vector<int> moves = gen_moves(b);

	for (int move : moves) {
		if (b.make_move(move)) {
			int score = -negamax(b, depth - 1, -beta, -alpha, nodes);
			if (score > alpha) {
				alpha = score;
				best_move = move;
			}

			nodes++;
		}

		b.unmake_move(move);
	}

	std::cout << nodes << " nodes searched\n";
	return best_move;
}


int negamax(Board &b, int depth, int alpha, int beta, int &nodes) {
	if (depth == 0) {
		return eval(b);
	}

	if (b.game_over()) {
		return eval(b, 1);
	}

	std::vector<int> moves = gen_moves(b);
	for (int move : moves) {
		if (b.make_move(move)) {
			int score = -negamax(b, depth - 1, -beta, -alpha, nodes);
			if (score > alpha) {
				alpha = score;
			}

			nodes++;

			if (alpha >= beta) {
				b.unmake_move(move);
				break;
			}
		}

		b.unmake_move(move);
	}

	return alpha;
}