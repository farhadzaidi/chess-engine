#include <vector>
#include <chrono>

#include "perft.hpp"
#include "movegen.hpp"

class Board;

int perft(Board &b, int depth) {
	int num_moves = 0;

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

void run_perft(Board &b, int depth) {
	std::cout << "\nMove Generation Performance Test\n";

	auto start = std::chrono::high_resolution_clock::now();
	int nodes = perft(b, depth);
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration<double>(end - start);

	std::cout << nodes << " nodes generated at depth " << depth
		<< " in " << elapsed.count() << " seconds\n";
}

