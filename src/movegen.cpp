#include "movegen.hpp"
#include "constants.hpp"
#include "board.hpp"
#include "move.hpp"

#include <iostream>

std::vector<int> gen_moves(Board &b) {
	std::vector<int> moves;
	for (int sq : b.piece_squares[b.to_move]) {
		switch (b.piece[sq]) {
			case PAWN:
				get_pawn_moves(b, sq, moves);
				break;
			case BISHOP:
				get_piece_moves(b, sq, BISHOP_MOVES, 1, moves);
				break;
			case KNIGHT:
				get_piece_moves(b, sq, KNIGHT_MOVES, 0, moves);
				break;
			case ROOK:
				get_piece_moves(b, sq, ROOK_MOVES, 1, moves);
				break;
			case QUEEN:
				get_piece_moves(b, sq, KING_QUEEN_MOVES, 1, moves);
				break;
			case KING:
				get_piece_moves(b, sq, KING_QUEEN_MOVES, 0, moves);
				break;
		}
	}

	return moves;
}

std::vector<int> validate_moves(Board &b, std::vector<int> moves) {
	std::vector<int> valid_moves;
	for (int move : moves) {
		if (b.make_move(move)) {
			valid_moves.push_back(move);
		}

		b.unmake_move(move);
	}

	return valid_moves;
}

void get_pawn_moves(Board &b, int sq, std::vector<int> &moves) {
	// Set directions based on pawn's perspective
	int is_white = b.color[sq] == WHITE ? 1 : 0;
	int north = is_white ? -10 : 10;
	int east = is_white ? 1 : -1;
	int west = is_white ? -1 : 1;

	// Determine starting row and promotion row based on pawn's perspective
	int start_row = is_white ? 6 : 1;
	int last_row = is_white ? 0 : 7;

	// Check if pawn can move north
	int north_sq = b.get_mailbox_num(sq, north);
	if (b.in_bounds(north_sq) && b.is_empty(north_sq)) {
		// Generate promotion moves
		if (north_sq / 8 == last_row) {
			// **Creating the queen promotion move first ensures autopromotion
			// to queen in the GUI
			moves.push_back(new_move(sq, north_sq, QUIET, P_QUEEN));
			moves.push_back(new_move(sq, north_sq, QUIET, P_BISHOP));
			moves.push_back(new_move(sq, north_sq, QUIET, P_KNIGHT));
			moves.push_back(new_move(sq, north_sq, QUIET, P_ROOK));
		} else {
			moves.push_back(new_move(sq, north_sq, QUIET, NORMAL));
		}

		// Check if pawn can move 2 squares north
		int north_north_sq = b.get_mailbox_num(sq, north + north);
		if (sq / 8 == start_row && b.is_empty(north_north_sq)) {
			moves.push_back(new_move(sq, north_north_sq, QUIET, NORMAL));
		}
	}

	// Check if pawn can take normally or en passant
	for (int dir : {north + east, north + west}) {
		int cap_sq = b.get_mailbox_num(sq, dir);
		if (b.in_bounds(cap_sq)) {
			// Normal pawn capture
			if (!b.is_empty(cap_sq) && b.diff_colors(sq, cap_sq)) {
				// Generate promotion moves
				if (cap_sq / 8 == last_row) {
					moves.push_back(new_move(sq, cap_sq, CAPTURE, P_QUEEN));
					moves.push_back(new_move(sq, cap_sq, CAPTURE, P_BISHOP));
					moves.push_back(new_move(sq, cap_sq, CAPTURE, P_KNIGHT));
					moves.push_back(new_move(sq, cap_sq, CAPTURE, P_ROOK));
				} else {
					moves.push_back(new_move(sq, cap_sq, CAPTURE, NORMAL));
				}
			// En passant capture
			} else if (cap_sq == b.enpas_sq.top()) {
				moves.push_back(new_move(sq, cap_sq, CAPTURE, EN_PASSANT));
			}
		}
	}
}

void get_piece_moves(
	Board &b, 
	int sq, 
	const int directions[8], 
	int slide,
	std::vector<int> &moves
) {
	for (int i = 0; i < 8; i++) {
		// Not all pieces have 8 move directions
		// Break the loop when the current piece has no more directions 
		if (directions[i] == 0) {
			break;
		}

		// Compute the next (to) square based on directions
		int nxt_sq = b.get_mailbox_num(sq, directions[i]);
		while (b.in_bounds(nxt_sq)) {
			// If the next square is empty, create the move and keep going
			if (b.is_empty(nxt_sq)) {
				moves.push_back(new_move(sq, nxt_sq, QUIET, NORMAL));
			} else {
				// Else check if the occupying piece is an enemy or friendly
				// piece to see if we can create a capture move
				if (b.diff_colors(sq, nxt_sq)) {
					moves.push_back(new_move(sq, nxt_sq, CAPTURE, NORMAL));
				} 
				break;
			}

			// No need to keep looping for non-sliding pieces (king/knight)
			if (!slide) {
				break;
			}

			nxt_sq = b.get_mailbox_num(nxt_sq, directions[i]);
		}
	}

	if (b.piece[sq] == KING) {
		int is_white = b.to_move == WHITE;
		int right_rook_start= is_white ? WRR_START : BRR_START;
		int left_rook_start = is_white ? WLR_START : BLR_START;
		int short_castling_right = is_white ? W_SHORT : B_SHORT;
		int long_castling_right = is_white ? W_LONG : B_LONG;

		// Ensure 3 conditions before creating castling move:
			// We have the respective castling right
			// The squares between the king and the respective rook are empty
			// The same-colored rook is at its starting position

		if (
				b.castling_rights & short_castling_right
				&& b.is_empty(b.king_squares[b.to_move] + 1)
				&& b.is_empty(b.king_squares[b.to_move] + 2)
				&& b.piece[right_rook_start] == ROOK
				&& b.color[right_rook_start] == b.to_move
			) {
			moves.push_back(new_move(sq, sq + 2, QUIET, CASTLE));
		}

		if (
				b.castling_rights & long_castling_right
				&& b.is_empty(b.king_squares[b.to_move] - 1)
				&& b.is_empty(b.king_squares[b.to_move] - 2)
				&& b.is_empty(b.king_squares[b.to_move] - 3)
				&& b.piece[left_rook_start] == ROOK
				&& b.color[left_rook_start] == b.to_move
			) {
			moves.push_back(new_move(sq, sq - 2, QUIET, CASTLE));
		}
	}
}