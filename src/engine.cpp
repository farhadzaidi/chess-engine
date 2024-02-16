#include <algorithm>

#include "constants.hpp"
#include "engine.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "trans_table.hpp"

/*
This file contains engine search and evaluation code. This is a search heavy
chess engine designed to search to the deepest depth possible in a short amount
of time. It utilizes the following techniques to prune the search tree as much
as possible:
	- Alpha-beta pruning
	- Move ordering
	- Iterative Deepening
	- Transposition table

Alpha-beta pruning works by pruning the tree at nodes where exploring further
will not change the outcome. For example, if black has a move that is +1
and white finds a move that is +3 (so -3 for black), we can stop search this
move variation since we know that black will never take the -3 move over the 
+1 move. The engine takes advantage of the fact that better moves lead to a faster
pruned tree, and so move ordering is vital. The transposition table caches
previously encountered positions and their evaluations so that we don't have
to recompute their evaluation if we encounter them again.

It is important to note that the engine spends a considerable amount of time 
conducting a quiescence search to stabilize the search and prevent the horizon 
effect (more on this later).
*/


// The engine utilizes a simple evaluation function focusing on material value
// while also accounting for checkmate/stalemate. The engine was designed this
// way to focus most of the computing power on the search to search to a deeper 
// depth as a more complex evaluation function would take more computational
// resources and thus reduce the overall search depth.
int eval(Board &b, int game_over) {
	if (game_over) {
		return b.is_attacked(b.king_squares[b.to_move]) ? -INF : 0;
	}

	int score = b.material[b.to_move] - b.material[!b.to_move];

	return score;
}

// Iterative deepening allows us to effectively prune the search tree, while
// ensuring the engine can end the search early (due to time constraints) and
// still have a valid move to play. It works by doing multiple searches, 
// increasing the search depth by 1 in each iteration. The pruning effect occurs
// because we explore the best move from the previous search first and since
// this is *usually* a good move, it should prune the search tree.
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

		prev_best_move = best_move;
	}

	return prev_best_move;
}

int search(Board &b, int depth, int prev_best_move, int &nodes, int &search_cancelled) {
	int alpha = -INF;
	int beta = INF;
	int best_move = 0;

	std::vector<int> moves = gen_moves(b);
	order_moves(moves);

	// Search the best move from the previous search first to prune the tree
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
			// Compute the score recursively until we reach depth 0 or explore
			// the maximum number of nodes. With every iteration, update the
			// lower bound for the score (alpha) and the respective best move.
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

// Negamax is a variation of the minimax algorithm that simplifies the code
// by negating the max score for each side upon every call. For example,
// a +1 score for white is the same as a -1 score for black, so we can take
// white's best score (let's say +1 in this case) and negate it to get black's
// best score for that particular move (which in this case is -1).
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

			// Check for transposition and return based on whether the node
			// has an exact evaluation or allows an alpha-beta cutoff
			if (b.trans_table.find(b.zobrist_hash) != b.trans_table.end()
				&& get_depth(b.trans_table[b.zobrist_hash]) >= depth) {

				if (get_node_type(b.trans_table[b.zobrist_hash]) == EXACT) {
					b.unmake_move(move);
					return get_eval(b.trans_table[b.zobrist_hash]);
				}

				if (get_node_type(b.trans_table[b.zobrist_hash]) == ALPHA
					&& get_eval(b.trans_table[b.zobrist_hash]) <= alpha) {
					b.unmake_move(move);
					return alpha;
				}

				if (get_node_type(b.trans_table[b.zobrist_hash]) == BETA
					&& get_eval(b.trans_table[b.zobrist_hash]) >= beta) {
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

			// If the move is better than the parent's best move, then perform
			// a beta-cutoff since the parent will not go down this path
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

// The quiscence search simply extends the original search by searching all
// captures until there are none left (or the maximum number of nodes are
// explored). This ensures the we don't make a move where the position seems
// but immediately becomes losing after a series of captures.
int quiescence_search(Board &b, int alpha, int beta, int &nodes, int &search_cancelled) {
	// Create an initial static evaluation in case we can't go any further
	int static_eval = eval(b);

	if (static_eval >= beta) {
		return beta;
	}

	if (static_eval > alpha) {
		alpha = static_eval;
	}

	std::vector<int> moves = gen_moves(b);
	order_moves(moves);

	// Same search logic, but this time we are only searching captures
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
