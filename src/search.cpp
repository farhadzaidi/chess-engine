#include <algorithm>
#include <random>

#include "constants.hpp"
#include "search.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "repr.hpp"
#include "move.hpp"

#include <iostream>

// Transposition table
	// Record all valid positions
	// Record depth and evaluation
	// Record node type
		// Record exact scores
		// Record lower bound values for all other nodes

int iterative_search(Board &b) {
	int search_cancelled = 0;
	int prev_best_move = 0;
	int best_move = 0;
	for (int i = 1; i <= INF; i++) {
		best_move = search(b, i, prev_best_move, search_cancelled);
		if (search_cancelled) {
			break;
		}

		prev_best_move = best_move;
	}

	std::cout << "\n";
	return prev_best_move;
}

int search(Board &b, int depth, int prev_best_move, int &search_cancelled) {
	int alpha = -INF;
	int beta = INF;
	int best_move = 0;
	int nodes = 0;

	std::vector<int> moves = gen_moves(b);
	order_moves(moves);

	if (prev_best_move) {
		nodes++;
		b.make_move(prev_best_move);
		alpha = -negamax(b, depth - 1, -beta, -alpha, nodes, search_cancelled);
		best_move = prev_best_move;
		b.unmake_move(prev_best_move);
	}

	for (int move : moves) {
		if (b.make_move(move)) {
			if (nodes > MAX_NODES) {
				search_cancelled = 1;
				b.unmake_move(move);
				return 0;
			}

			nodes++;
			int score = -negamax(b, depth - 1, -beta, -alpha, nodes, search_cancelled);

			if (score > alpha) {
				alpha = score;
				best_move = move;
			}
		}

		b.unmake_move(move);
	}

	std::cout << nodes << " nodes searched at depth " << depth << "\n"; 
	return best_move;
}


int negamax(Board &b, int depth, int alpha, int beta, int &nodes, int &search_cancelled) {
	if (search_cancelled) {
		return 0;
	}

	if (b.game_over()) {
		return eval(b, 1);
	}

	if (depth == 0) {
		return quiescence_search(b, alpha, beta, nodes, search_cancelled);
	}

	std::vector<int> moves = gen_moves(b);
	order_moves(moves);

	for (int move : moves) {
		if (b.make_move(move)) {
			if (nodes > MAX_NODES) {
				search_cancelled = 1;
				b.unmake_move(move);
				return 0;
			}

			nodes++;
			int score = -negamax(b, depth - 1, -beta, -alpha, nodes, search_cancelled);

			if (score >= beta) {
				b.unmake_move(move);
				return beta;
			}

			if (score > alpha) {
				alpha = score;
			}
		}

		b.unmake_move(move);
	}

	return alpha;
}


int quiescence_search(Board &b, int alpha, int beta, int &nodes, int &search_cancelled) {
	if (search_cancelled) {
		return 0;
	}

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
				if (nodes > MAX_NODES) {
					search_cancelled = 1;
					b.unmake_move(move);
					return 0;
				}

				nodes++;
				int score = -quiescence_search(b, -beta, -alpha, nodes, search_cancelled);

				if (score >= beta) {
					b.unmake_move(move);
					return beta;
				}

				if (score > alpha) {
					alpha = score;
				}
			}

			b.unmake_move(move);
		}
	}

	return alpha;
}


int move_comparator(const int &m1, const int &m2) {
	// Order by capture
	if (get_mtype(m1) == CAPTURE && get_mtype(m2) != CAPTURE) {
		return 1;
	}

	return 0;
}

void order_moves(std::vector<int> &moves, int prev_best_move) {
	std::sort(moves.begin(), moves.end(), move_comparator);
}
