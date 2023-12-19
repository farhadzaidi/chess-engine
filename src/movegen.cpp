#include "movegen.h"

std::vector<int> get_pawn_moves(Board &b, int sq) {
	std::vector<int> moves;

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
			moves.push_back(new_move(sq, north_sq, QUIET, P_BISHOP));
			moves.push_back(new_move(sq, north_sq, QUIET, P_KNIGHT));
			moves.push_back(new_move(sq, north_sq, QUIET, P_ROOK));
			moves.push_back(new_move(sq, north_sq, QUIET, P_QUEEN));
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
	int cap_directions[] = {north + east, north + west};
	for (int cap_dir : cap_directions) {
		int cap_sq = b.get_mailbox_num(sq, cap_dir);
		if (b.in_bounds(cap_sq)) {
			if (b.is_empty(cap_sq) && b.diff_colors(sq, nxt_sq)) {
				// Generate promotion moves
				if (cap_sq / 8 == last_row) {
					moves.push_back(new_move(sq, cap_sq, QUIET, P_BISHOP));
					moves.push_back(new_move(sq, cap_sq, QUIET, P_KNIGHT));
					moves.push_back(new_move(sq, cap_sq, QUIET, P_ROOK));
					moves.push_back(new_move(sq, cap_sq, QUIET, P_QUEEN));
				} else {
					moves.push_back(new_move(sq, cap_sq, CAPTURE, NORMAL));
				}
			} else if (cap_sq == b.enpas_sq) {
				moves.push_back(new_move(sq, cap_sq, CAPTURE, EN_PASSANT));
			}
		}
	}

	return moves;
}

std::vector<int> get_piece_moves(Board &b, int sq, int directions, int sliding) {
	std::vector<int> moves;
	for (int dir : directions) {
		// Compute the next (move to) square based on directions
		int nxt_sq = b.get_mailbox_num(sq, dir);
		while (b.in_bounds(nxt_sq)) {
			// If the next square is empty, create the move and keep going
			if (b.is_empty(nxt_sq)) {
				moves.push_back(new_move(sq, nxt_sq, QUIET, NORMAL));
			} else {
				// If the next square is occupied, handle based on the color
				// of the occupying piece
				if (b.diff_colors(sq, nxt_sq)) {
					moves.push_back(new_move(sq, nxt_sq, CAPTURE, NORMAL));
				}
				break;
			}

			// Keep going if the piece is sliding, else break
			if (!sliding) {
				break;
			}

			// Iterate nxt_sq for sliding pieces
			nxt_sq = b.get_mailbox_num(nxt_sq, dir);
		}
	}

	// Generate castling moves
	

	return moves;
}