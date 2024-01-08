#include <vector>
#include <iostream>
#include <string>

#include "perft.hpp"
#include "constants.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "repr.hpp"

class Board;

unsigned long long perft(int depth, Board &b, int original_depth) {
	unsigned long long num_moves = 0;

	if (depth == 0) {
		return 1ULL;
	}

	std::vector<int> moves = gen_all_moves(b);
	for (int move : moves) {
		if (b.make_move(move)) {
			num_moves += perft(depth - 1, b, original_depth);
		}

		b.unmake_move(move);
	}

	return num_moves;
}

