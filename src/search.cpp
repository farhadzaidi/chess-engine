#include <random>
#include <algorithm>
#include <vector>
#include <iostream>

#include "search.hpp"
#include "constants.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"

int random_move(Board &b) {
	std::vector<int> valid_moves = validate_moves(b, gen_moves(b));

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(valid_moves.begin(), valid_moves.end(), gen);

	return valid_moves[0];
}

Move_Eval minimax(Board &b, int depth, int alpha, int beta) {
	if (depth == 0) {
		return Move_Eval(b.move_list.top(), eval(b));
	}

	if (b.game_over()) {
		return Move_Eval(0, eval(b));
	}

	int side = b.to_move;
	std::vector<int> moves = gen_moves(b);
	Move_Eval best(0, side == WHITE ? -9999 : 9999);
	for (int move : moves) {
		if (b.make_move(move)) {
			const Move_Eval &cur = minimax(b, depth - 1, alpha, beta);

			if (side == WHITE) {
				if (cur.eval > best.eval) {
					best.move = move;
					best.eval = cur.eval;
				}

				alpha = std::max(alpha, cur.eval);
				if (beta <= alpha) {
					b.unmake_move(move);
					break;
				}
			} else {
				if (cur.eval < best.eval) {
					best.move = move;
					best.eval = cur.eval;
				}

				beta = std::min(beta, cur.eval);
				if (beta <= alpha) {
					b.unmake_move(move);
					break;
				}
			}
		}

		b.unmake_move(move);
	}

	return best;
}

			// if (side == WHITE && cur.eval > best.eval) {
			// 	best.move = move;
			// 	best.eval = cur.eval;
			// } else if (side == BLACK && cur.eval < best.eval) {
			// 	best.move = move;
			// 	best.eval = cur.eval;
			// }