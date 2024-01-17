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
	int max_score = -INF;
	int best_move = 0;

	for (int move : moves) {
		if (b.make_move(move)) {
			int score = -negamax(b, depth - 1, -beta, -alpha);
			if (score > max_score) {
				max_score = score;
				alpha = score;
				best_move = move;
			}
		}

		b.unmake_move(move);
	}

	return best_move;
}


int negamax(Board &b, int depth, int alpha, int beta) {
	if (depth == 0 || b.game_over()) {
		return eval(b);
	}

	std::vector<int> moves = gen_moves(b);
	int max_score = -INF;
	for (int move : moves) {
		if (b.make_move(move)) {
			int score = -negamax(b, depth - 1, -beta, -alpha);
			if (score > max_score) {
				max_score = score;
				alpha = score;
			}

			if (alpha >= beta) {
				b.unmake_move(move);
				break;
			}
		}

		b.unmake_move(move);
	}

	return max_score;
}

// Transposition table:
	// cache every position and its evaluation in the table
	// after making a move, check if the position is in the table
		// if so, return the evaluation of the position

// void order_moves(std::vector<int> &moves) {}

