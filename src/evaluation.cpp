#include "constants.hpp"
#include "evaluation.hpp"
#include "board.hpp"

// TODO:
	// Piece maps
	// King safety
	// Pawn structure

int eval(Board &b) {
	return b.material[b.to_move] - b.material[!b.to_move];
}
