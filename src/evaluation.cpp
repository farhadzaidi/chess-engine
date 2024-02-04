#include "constants.hpp"
#include "evaluation.hpp"
#include "board.hpp"
#include "move.hpp"
#include "repr.hpp"

#include <iostream>

// TODO:
	// Remove king penalties in the endgame
	// Encourage pushing enemy king into corner of the board

int eval(Board &b, int game_over) {
	// Evaluate for checkmate or stalemate
	if (game_over) {
		return b.is_attacked(b.king_squares[b.to_move]) ? -INF : 0;
	}

	// Material (dominant attribute)
	int score = b.material[b.to_move] - b.material[!b.to_move];

	return score;
}
