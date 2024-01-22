#include "constants.hpp"
#include "search.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"

#include <iostream>

// Search uses the minimax algorithm with the following pruning algorithms:
	// Alpha-beta pruning

// TODO:
	// Move ordering
	// Transposition tables
	// Iterative deepening
	// Quiescence search
	// Late move reductions
	// Aspiration windows
	// Time search (instead of fixed depth

int search(Board &b, int depth) {
	int alpha = -INF;
	int beta = INF;
	std::vector<int> moves = gen_moves(b);
	int max_score = -INF;
	int best_move = 0;
	int nodes = 0;

	for (int move : moves) {
		if (b.make_move(move)) {
			nodes++;
			int score = -negamax(b, depth - 1, -beta, -alpha, nodes);
			b.trans_table.emplace(b.zobrist_hash, ttEntry(depth, score));
			
			if (score > max_score) {
				max_score = score;
				alpha = score;
				best_move = move;
			}
		}

		b.unmake_move(move);
	}

	std::cout << nodes << " nodes searched at depth " << ENGINE_DEPTH << ".\n";
	return best_move;
}


int negamax(Board &b, int depth, int alpha, int beta, int &nodes) {
	if (depth == 0 || b.game_over()) {
		return eval(b);
	}

	std::vector<int> moves = gen_moves(b);
	int max_score = -INF;
	for (int move : moves) {
		if (b.make_move(move)) {
			nodes++;
			// Stop search if transposition encountered and return previously
			// encountered score of transposition
			if (b.trans_table.find(b.zobrist_hash) != b.trans_table.end()) {
				b.unmake_move(move);
				return b.trans_table[b.zobrist_hash].evaluation;
			}

			int score = -negamax(b, depth - 1, -beta, -alpha, nodes);
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
//