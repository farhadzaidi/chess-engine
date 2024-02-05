#include <algorithm>
#include <random>

#include "constants.hpp"
#include "search.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "repr.hpp"
#include "move.hpp"
#include "trans_table.hpp"

#include <iostream>

// Transposition table
	// Record all valid positions
	// Record depth and evaluation
	// Record node type
		// Record exact scores
		// Record lower bound values for all other nodes

// How to use TT
	// If we encounter a fully searched exact node, we just return its value
	// If we encounter a cut-node, that means that the evaluation of this node
	// caused a beta cutoff, and we returned beta. This is an UPPER BOUND (since
	// beta is an upper bound) and as such, we should update the current beta
	// value and stop searching this node
	// If we encounter an all-node, this means that this node did not improve
	// the lower bound (alpha) and as such there is no need to search this
	// node

	// This only works assuming that the depth of the node is >= to current
	// depth
	// There are ways to use nodes with lower depth but im not sure yet

int iterative_search(Board &b) {
	int search_cancelled = 0;
	int prev_best_move = 0;
	int best_move = 0;

	for (int i = 1; i <= INF; i++) {
		int nodes = 0;
		best_move = search(b, i, prev_best_move, nodes, search_cancelled);
		if (search_cancelled) {
			break;
		}
		std::cout << nodes << " nodes searched at depth " << i << "\n";
		prev_best_move = best_move;
	}

	std::cout << "\n";
	return prev_best_move;
}

int search(Board &b, int depth, int prev_best_move, int &nodes, int &search_cancelled) {
	int alpha = -INF;
	int beta = INF;
	int best_move = 0;

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

	return best_move;
}


int negamax(Board &b, int depth, int alpha, int beta, int &nodes, int &search_cancelled) {
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
			// Check is max number of nodes has been exceeded
			// If so, set search_cancelled to true and return 0 (garbage value)
			if (nodes > MAX_NODES) {
				search_cancelled = 1;
				b.unmake_move(move);
				return 0;
			}

			int node_type = ALPHA;
			nodes++;

			if (b.trans_table.find(b.zobrist_hash) != b.trans_table.end()
				&& get_depth(b.trans_table[b.zobrist_hash]) >= depth) {

				if (get_node_type(b.trans_table[b.zobrist_hash]) == EXACT) {
					std::cout << "TT HIT EXACT\n";
					b.unmake_move(move);
					return get_eval(b.trans_table[b.zobrist_hash]);
				}

				if (get_node_type(b.trans_table[b.zobrist_hash]) == ALPHA
					&& get_eval(b.trans_table[b.zobrist_hash]) <= alpha) {
					std::cout << "TT HIT ALPHA\n";
					b.unmake_move(move);
					return alpha;
				}

				if (get_node_type(b.trans_table[b.zobrist_hash]) == BETA
					&& get_eval(b.trans_table[b.zobrist_hash]) >= beta) {
					std::cout << "TT HIT BETA\n";
					b.unmake_move(move);
					return beta;
				}
			}

			int score = -negamax(b, depth - 1, -beta, -alpha, nodes, search_cancelled);
			// Ensure that the score is valid and not simply garbage due to the
			// search being cancelled
			if (search_cancelled) {
				b.unmake_move(move);
				return 0;
			}

			if (score >= beta) {
				b.unmake_move(move);
				b.trans_table[b.zobrist_hash] = new_tt_entry(BETA, depth, beta);
				return beta;
			}

			if (score > alpha) {
				alpha = score;
				node_type = EXACT;
			}

			b.trans_table[b.zobrist_hash] = new_tt_entry(node_type, depth, alpha);
		}

		b.unmake_move(move);
	}


	return alpha;
}


int quiescence_search(Board &b, int alpha, int beta, int &nodes, int &search_cancelled) {
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
				if (search_cancelled) {
					b.unmake_move(move);
					return 0;
				}

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
