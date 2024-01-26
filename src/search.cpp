#include <algorithm>

#include "constants.hpp"
#include "search.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "repr.hpp"
#include "move.hpp"

#include <iostream>

// Alpha-beta pruning algorithm
	// Alpha is the lower bound on the evaluation of the current node
		// If we encounter a move that leads to a score greater than
		// alpha then we overwrite alpha with that score.
	
	// Beta is the upper bound on the evaluation of the current node and the
	// lower bound on the evaluation of the parent node.
	// We cannot have alpha be greater than beta, since that would lead to an
	// evaluation lower than the parent's lower bound (alpha).
		// Anytime we encounter a move that leads to a score greater
		// beta, we can stop looking further and prune the tree, since
		// this move will never be played.

int search(Board &b, int depth) {
	int alpha = -INF;
	int beta = INF;
	int best_move = 0;
	int nodes = 0;

	std::vector<int> moves = gen_moves(b);
	order_moves(moves);

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
	if (b.game_over()) {
		return eval(b, 1);
	}

	if (depth == 0) {
		return quiescence_search(b, alpha, beta, nodes);
	}

	std::vector<int> moves = gen_moves(b);
	order_moves(moves);

	for (int move : moves) {
		if (b.make_move(move)) {
			int score = -negamax(b, depth - 1, -beta, -alpha, nodes);

			if (score >= beta) {
				b.unmake_move(move);
				return beta;
			}

			if (score > alpha) {
				alpha = score;
			}

			nodes++;
		}

		b.unmake_move(move);
	}

	return alpha;
}


int quiescence_search(Board &b, int alpha, int beta, int &nodes) {
	int static_eval = eval(b);

	if (static_eval >= beta) {
		return beta;
	}

	if (static_eval > alpha) {
		alpha = static_eval;
	}

	std::vector<int> moves = gen_moves(b);
	order_moves(moves);

	for (int move : moves) {
		if (get_mtype(move) == CAPTURE) {
			if (b.make_move(move)) {
				int score = -quiescence_search(b, -beta, -alpha, nodes);

				if (score >= beta) {
					b.unmake_move(move);
					return beta;
				}

				if (score > alpha) {
					alpha = score;
				}

				nodes++;
			}

			b.unmake_move(move);
		}
	}

	return alpha;
}


int move_comparator(const int &m1, const int &m2) {
	if (get_mtype(m1) == CAPTURE && get_mtype(m2) != CAPTURE) {
		return 1;
	} else if (get_mtype(m1) != CAPTURE && get_mtype(m2) == CAPTURE) {
		return 1;
	}

	return 0;
}

void order_moves(std::vector<int> &moves) {
	std::sort(moves.begin(), moves.end(), move_comparator);
}