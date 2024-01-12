#include <vector>

#include "constants.hpp"
#include "search.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"

// Search uses the minimax algorithm with the following pruning algorithms:
	// Alpha-beta pruning

// TODO:
	// Move ordering
	// Transposition tables
	// Iterative deepening
	// Quiescence search
	// Late move reductions
	// Aspiration windows
	// Time search (instead of fixed depth)

int search(Board &b, int depth) {
	int alpha = -INF;
	int beta = INF;
	std::vector<int> moves = gen_moves(b);
	int best_move = 0;

	if (b.to_move == WHITE) {
		int best_score = -INF;
		for (int move : moves) {
			if (b.make_move(move)) {
				int cur_score = mini(b, depth - 1, alpha, beta);
				if (cur_score > best_score) {
					best_score = cur_score;
					alpha = cur_score;
					best_move = move;
				}
			}

			b.unmake_move(move);
		}
	} else {
		int best_score = INF;
		for (int move : moves) {
			if (b.make_move(move)) {
				int cur_score = maxi(b, depth - 1, alpha, beta);
				if (cur_score < best_score) {
					best_score = cur_score;
					beta = cur_score;
					best_move = move;
				}
			}

			b.unmake_move(move);
		}
	}

	return best_move;
}

int mini(Board &b, int depth, int alpha, int beta) {
	if (depth == 0 || b.game_over()) {
		return eval(b);
	}

	int min_score = INF;
	std::vector<int> moves = gen_moves(b);
	for (int move: moves) {
		if (b.make_move(move)) {
			int cur_score = maxi(b, depth - 1, alpha, beta);
			if (cur_score < min_score) {
				min_score = cur_score;
				beta = cur_score;
			}

			if (alpha >= beta) {
				b.unmake_move(move);
				break;
			}
		}

		b.unmake_move(move);
	}

	return min_score;
}

int maxi(Board &b, int depth, int alpha, int beta) {
	if (depth == 0 || b.game_over()) {
		return eval(b);
	}

	int max_score = -INF;
	std::vector<int> moves = gen_moves(b);
	for (int move: moves) {
		if (b.make_move(move)) {
			int cur_score = mini(b, depth - 1, alpha, beta);
			if (cur_score > max_score) {
				max_score = cur_score;
				alpha = cur_score;
			}

			if (beta <= alpha) {
				b.unmake_move(move);
				break;
			}
		}

		b.unmake_move(move);
	}

	return max_score;
}