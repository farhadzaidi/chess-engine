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

std::pair<int, int> minimax(Board &b, int depth) {
	if (depth == 0) {
		return {b.move_list.top(), eval(b)};
	}

	std::vector<int> moves = gen_moves(b);
	int side = b.to_move;
	int dummy_eval = side == WHITE ? -9999 : 9999;

	std::pair<int, int> best_move_eval = {0, dummy_eval};
	for (int move : moves) {
		if (b.make_move(move)) {
			std::pair<int, int> move_eval = minimax(b, depth - 1);
			if (side == WHITE 
					&& move_eval.second >= best_move_eval.second) {
				best_move_eval.first = move;
				best_move_eval.second = move_eval.second;
			} else if (side == BLACK
					&& move_eval.second <= best_move_eval.second){
				best_move_eval.first = move;
				best_move_eval.second = move_eval.second;
			}
		}

		b.unmake_move(move);
	}

	return best_move_eval;
}