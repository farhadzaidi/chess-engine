#include <string>

#include "board.hpp"
#include "repr.hpp"
#include "perft.hpp"
#include "gui.hpp"

int main(int argc, char* argv[]) {
	// Initialize board
	std::string FEN = "8/4k3/8/5Q2/5K2/8/8/8 w - - 0 1";
	Board b;
	load_from_FEN(b, FEN);

	// Perft
	if (argc > 2 && std::string(argv[1]).compare("perft") == 0) {
		int depth = std::stoi(argv[2]);
		run_perft(b, depth);
	} else { // GUI
		run_gui(b);
	}

    return 0;
}


