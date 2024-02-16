#include <vector>
#include <chrono>

#include "perft.hpp"
#include "movegen.hpp"

class Board;

/*

This file performs a move generation performance test (perft) to not only test
the speed of the engine's move generating, but to also ensure that the engine
is generating moves correctly. We can confirm this by counting the number of 
moves generated at different depths in various positions and comparing our
results to the consensus results agreed upon by the chess programming community.

*/

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

