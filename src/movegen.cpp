#include "movegen.hpp"
#include "constants.hpp"
#include "board.hpp"
#include "move.hpp"

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
	for (int dir : {north + east, north + west}) {
		int cap_sq = b.get_mailbox_num(sq, dir);
		if (b.in_bounds(cap_sq)) {
			// Normal pawn capture
			if (!b.is_empty(cap_sq) && b.diff_colors(sq, cap_sq)) {
				// Generate promotion moves
				if (cap_sq / 8 == last_row) {
					moves.push_back(new_move(sq, cap_sq, QUIET, P_BISHOP));
					moves.push_back(new_move(sq, cap_sq, QUIET, P_KNIGHT));
					moves.push_back(new_move(sq, cap_sq, QUIET, P_ROOK));
					moves.push_back(new_move(sq, cap_sq, QUIET, P_QUEEN));
				} else {
					moves.push_back(new_move(sq, cap_sq, CAPTURE, NORMAL));
				}
			// en passant capture
			} else if (cap_sq == b.enpas_sq) {
				moves.push_back(new_move(sq, cap_sq, CAPTURE, EN_PASSANT));
			}
		}
	}

	return moves;
}

std::vector<int> get_sliding_moves(Board &b, int sq, int directions[8]) {
	// Keep track of attack maps for each piece
		// unordered_map mapping piece square to unordered_set of attacked squares
		// Can check if a king is in check if the king's index is in any of the attack maps
		// (will have to loop thru each enemy piece, but this shouldn't be much overhead)
	// Before move generation, at the start of every position,
	// find friendly pieces nearest to the king in each direction
	// once a piece is found, check if there is an attacking piece
	// in that ray (e.g. if the friendly piece is somewhere northeast
	// of the king, check if there is a bishop or queen farther northeast)
		// if so, this is a pinned piece and can only be moved in that direction
		// (e.g. northeast)
			// Need to know 2 things:
				// piece square
				// pin direction
			// Similar to attack map, map piece square to pin direction
	// At worst, this will create rays from the king spanning to the end of board,
	// once before every move gen call (slight overhead)
	// During this check, we can kill 2 birds with 1 stone
		// Check if run into an enemy sliding piece that can attack the king
		// if so, the king is in check
			// even though we have attack maps, this will account for discovered checks
	// To check if a capture is valid while the king is in check, ignore the captured piece's
	// attack map when checking for checks

	// This data gets reset every ply

	std::vector<int> moves;
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
				if (b.diff_colors(sq, nxt_sq)) {
					moves.push_back(new_move(sq, nxt_sq, CAPTURE, NORMAL));
				} 
				break;
			}

			nxt_sq = b.get_mailbox_num(sq, directions[i]);
		}
	}

	return moves;
}