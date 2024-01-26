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

	// Give some extra points for positions where pieces are being mobolized:
	// for (int sq : b.piece_squares[b.to_move]) {
	// 	switch (b.piece[sq]) {
	// 		// Pawns pushing up the board
	// 		case PAWN: {
	// 			score += PAWN_EVAL_MAP[b.to_move][sq];
	// 			break;
	// 		}
	// 		// Knights taking up outposts
	// 		case KNIGHT: {
	// 			score += KNIGHT_EVAL_MAP[sq];
	// 			break;				
	// 		}
	// 		// Bishops covering as many squares as possible
	// 		case BISHOP: {
	// 			// Light squares have the same parity as the row they are on
	// 			int row = sq / 8;
	// 			int btype = (row & 1) == (sq & 1) ? 0 : 1;
	// 			score += BISHOP_EVAL_MAP[btype][sq];
	// 			break;				
	// 		}
	// 		// King safe in the corner (early game)
	// 		case KING: {
	// 			int num_pieces = b.piece_squares[WHITE].size() + b.piece_squares[BLACK].size();
	// 			if (num_pieces > ENDGAME_THRESHOLD) {
	// 				score += KING_EVAL_MAP[b.to_move][sq];
	// 			}
	// 			break;
	// 		}
	// 	}
	// }

	// Penalize getting checked
	// if (b.is_attacked(b.king_squares[b.to_move])) {
	// 	score -= 10;
	// }

	return score;
}
