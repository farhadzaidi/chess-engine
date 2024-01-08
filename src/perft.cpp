#include <vector>

#include "perft.hpp"
#include "movegen.hpp"

class Board;

unsigned long long perft(Board &b, int depth) {
	unsigned long long num_moves = 0;

	if (depth == 0) {
		return 1ULL;
	}

	std::vector<int> moves = gen_moves(b);
	for (int move : moves) {
		if (b.make_move(move)) {
			num_moves += perft(b, depth - 1);
		}

		b.unmake_move(move);
	}

	return num_moves;
}

