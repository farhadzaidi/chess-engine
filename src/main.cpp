#include <string>

#include "board.hpp"
#include "repr.hpp"
#include "perft.hpp"
#include "gui.hpp"

int main(int argc, char* argv[]) {
	// Initialize board
	// std::string FEN = "7k/8/8/8/5B2/8/8/6K1 w - - 0 1";	
	std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	// std::string FEN = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
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


