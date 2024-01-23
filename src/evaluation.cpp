#include "constants.hpp"
#include "evaluation.hpp"
#include "board.hpp"
#include "move.hpp"
#include <iostream>

// Piece maps
	// Pawns want to be closer to the other side of the board
	// Knights and Bishops want to be closer to the center of the board
	// Rooks should be connected (?)
	// Queen should be somewhat in the action (ranks 2-4)
	// King wants to be safe at the corners of the board
		// King piece map is gonna change in the endgame

// Rewards checks (especially in the endgame)
// Give a -INF score for checkmate and 0 score for stalemate

// Pawn eval maps focus on pushing pawns to promotion

int eval(Board &b, int game_over) {
	if (game_over) {
		return b.is_attacked(b.king_squares[b.to_move]) ? -INF : 0;
	}

	// Material (dominant attribute)
	int score = b.material[b.to_move] - b.material[!b.to_move];

	// Give some extra points for positions where pieces are being mobolized
	// (e.g. king is safe in the corner, knights in the middle, etc.)
	for (int sq : b.piece_squares[b.to_move]) {
		switch (b.piece[sq]) {
			case PAWN: {
				score += PAWN_EVAL_MAP[b.to_move][sq];
				break;
			}
			case KNIGHT: {
				score += KNIGHT_EVAL_MAP[sq];
				break;				
			}
			case BISHOP: {
				// Light squares have the same parity as the row they are on
				int row = sq / 8;
				int btype = (row & 1) == (sq & 1) ? 0 : 1;
				score += BISHOP_EVAL_MAP[btype][sq];
				break;				
			}
			case KING: {
				score += KING_EVAL_MAP[b.to_move][sq];
				break;
			}
		}
	}

	// TODO: Boost score for castling

	return score;
}
