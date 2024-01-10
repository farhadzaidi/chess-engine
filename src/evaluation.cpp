#include "constants.hpp"
#include "evaluation.hpp"
#include "board.hpp"

int eval(Board &b) {
	int white_score = 0;
	for (int sq : b.piece_squares[WHITE]) {
		white_score += PIECE_VALUE[b.piece[sq]];
	}

	int black_score = 0;
	for (int sq : b.piece_squares[BLACK]) {
		black_score += PIECE_VALUE[b.piece[sq]];
	}

	return white_score - black_score;
}
