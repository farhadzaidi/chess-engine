#include "board.hpp"
#include "constants.hpp"
#include "move.hpp"

#include <iostream>
#include <cmath>

Board::Board() {}

// TODO: split up make_move and unmake_move into different functions

int Board::make_move(int move) {
	int from = get_from(move);
	int to = get_to(move);
	int mtype = get_mtype(move);
	int flag = get_flag(move);
	int valid = 1;

	// Set en passant target square if pawn moves two squares
	int south = to_move == WHITE ? 8 : -8;
	if (piece[from] == PAWN && abs(from - to) == 16) {
		enpas_sq.push(to + south);
	} else {
		enpas_sq.push(-1);
	}

	if (mtype == CAPTURE) {
		// Change square of captured piece to south of "to" if the move type is
		// en passant
		int cap_sq = to;
		if (flag == EN_PASSANT) {
			cap_sq += south;
			captured_pieces[to_move].push(piece[cap_sq]);
			piece[cap_sq] = EMPTY;
			color[cap_sq] = EMPTY;
		} else {
			captured_pieces[to_move].push(piece[cap_sq]);
		}

		piece_squares[!to_move].erase(cap_sq);
	}

	piece_squares[to_move].erase(from);
	piece_squares[to_move].insert(to);

	piece[to] = piece[from];
	color[to] = color[from];
	piece[from] = EMPTY;
	color[from] = EMPTY;

	if (flag == CASTLE) {
		valid &= !is_attacked(from, to_move);

		int new_rook_pos;
		int cur_rook_pos;
		// Long castle
		if (from > to) {
			valid &= !is_attacked(from - 1, to_move);
			new_rook_pos = to + 1;
			cur_rook_pos = to_move == WHITE ? WLR_START : BLR_START;
		} else { // Short castle
			valid &= !is_attacked(from + 1, to_move);
			new_rook_pos = to - 1;
			cur_rook_pos = to_move == WHITE ? WRR_START : BRR_START;
		}

		piece[new_rook_pos] = ROOK;
		color[new_rook_pos] = to_move;
		piece[cur_rook_pos] = EMPTY;
		color[cur_rook_pos] = EMPTY;
		piece_squares[to_move].erase(cur_rook_pos);
		piece_squares[to_move].insert(new_rook_pos);
	}

	// Change piece type to promotion piece, if applicable
	switch (flag) {
		case P_BISHOP:
			piece[to] = BISHOP;
			break;
		case P_KNIGHT:
			piece[to] = KNIGHT;
			break;
		case P_ROOK:
			piece[to] = ROOK;
			break;
		case P_QUEEN:
			piece[to] = QUEEN;
			break;
	}

	update_castling_rights(piece[to], to_move);

	if (piece[to] == KING) {
		king_squares[to_move] = to;
	}

	// Validate move
	valid &= !is_attacked(king_squares[to_move], to_move);

	// Switch turn
	to_move = !to_move;

	return valid;
}

void Board::unmake_move(int move) {
	int from = get_from(move);
	int to = get_to(move);
	int mtype = get_mtype(move);
	int flag = get_flag(move);

	// Revert turn
	to_move = !to_move;

	enpas_sq.pop();

	// Unmake normal move
	piece[from] = piece[to];
	color[from] = color[to];
	piece_squares[to_move].erase(to);
	piece_squares[to_move].insert(from);

	if (mtype == CAPTURE) {
		int south = to_move == WHITE ? 8 : -8;
		int cap_sq = flag == EN_PASSANT ? to + south : to;

		piece_squares[!to_move].insert(cap_sq);
		int captured_piece = captured_pieces[to_move].top();
		captured_pieces[to_move].pop();
		piece[cap_sq] = captured_piece;
		color[cap_sq] = !to_move;
	}

	if (flag == CASTLE) {
		// Same as in make_move, just switch current and new rook positions
		int new_rook_pos;
		int cur_rook_pos;
		// Long castle
		if (from > to) {
			new_rook_pos = to_move == WHITE ? WLR_START : BLR_START;
			cur_rook_pos = to + 1;

		} else { // Short castle
			new_rook_pos = to_move == WHITE ? WRR_START : BRR_START;
			cur_rook_pos = to - 1;
		}

		piece[new_rook_pos] = ROOK;
		color[new_rook_pos] = to_move;
		piece[cur_rook_pos] = EMPTY;
		color[cur_rook_pos] = EMPTY;
		piece_squares[to_move].erase(cur_rook_pos);
		piece_squares[to_move].insert(new_rook_pos);
	}

	// If the flag is a promotion of any kind, change the piece at 
	// "from" to a pawn
	if (flag == P_BISHOP || flag == P_KNIGHT || flag == P_ROOK || flag == P_QUEEN) {
		piece[from] = PAWN;
	}

	// Only empty "to" for quiet moves or en passant captures
	// For regular captures, the captured piece will go back to "to"
	if (mtype == QUIET || flag == EN_PASSANT) {
		piece[to] = EMPTY;
		color[to] = EMPTY;
	}

	int castling_rights_update = castling_rights_updates.top();
	castling_rights_updates.pop();
	castling_rights |= castling_rights_update;

	if (piece[from] == KING) {
		king_squares[to_move] = from;
	}
}

void Board::update_castling_rights(int moving_piece, int side) {
	int update = 0;
	if (castling_rights) {
		int short_right = side == WHITE ? W_SHORT : B_SHORT;
		int long_right = side == WHITE ? W_LONG : B_LONG;
		int rr_start = side == WHITE ? WRR_START : BRR_START;
		int lr_start = side == WHITE ? WLR_START : BLR_START;

		if (moving_piece == KING) {
			if (castling_rights & short_right) {
				castling_rights ^= short_right;
				update |= short_right;
			}

			if (castling_rights & long_right) {
				castling_rights ^= long_right;
				update |= long_right;
			}
		}

		if (moving_piece == ROOK) {
			if (castling_rights & short_right && is_empty(rr_start)) {
				castling_rights ^= short_right;
				update |= short_right;
			}

			if (castling_rights & long_right && is_empty(lr_start)) {
				castling_rights ^= long_right;
				update |= long_right;
			}	
		}
	}
	castling_rights_updates.push(update);
}

int Board::is_attacked(int sq, int side) {
	const int DIAGNAL_ATTACKERS[2] = {QUEEN, BISHOP};
	if (is_attacked_helper(sq, BISHOP_MOVES, 1, DIAGNAL_ATTACKERS, !side))
		return 1;

	const int STRAIGHT_ATTACKERS[2] = {QUEEN, ROOK};
	if (is_attacked_helper(sq, ROOK_MOVES, 1, STRAIGHT_ATTACKERS, !side))
		return 1;

	const int KNIGHT_ATTACKER[2] = {KNIGHT, 0};
	if (is_attacked_helper(sq, KNIGHT_MOVES, 0, KNIGHT_ATTACKER, !side))
		return 1;

	const int KING_ATTACKER[2] = {KING, 0};
	if (is_attacked_helper(sq, KING_QUEEN_MOVES, 0, KING_ATTACKER, !side))
		return 1;

	const int PAWN_ATTACKER[2] = {PAWN, 0};
	const int PAWN_ATTACKS[8] = {
	    (side == WHITE) ? -11 : 11,
	    (side == WHITE) ? -9 : 9,
	    0, 0, 0, 0, 0, 0
	};

	return is_attacked_helper(sq, PAWN_ATTACKS, 0, PAWN_ATTACKER, !side);
}

int Board::is_attacked_helper(int sq, const int directions[8], int slide, 
	const int attackers[2], int attacking_side) {
	for (int i = 0; i < 8; i++) {
		if (directions[i] == 0) {
			break;
		}

		int nxt_sq = get_mailbox_num(sq, directions[i]);
		while (in_bounds(nxt_sq)) {
			if (!is_empty(nxt_sq)) {
				if (color[nxt_sq] == attacking_side) {
					for (int j = 0; j < 2; j++) {
 						if (piece[nxt_sq] == attackers[j]) {
							return 1;
						}
					}
				}

				break;
			}

			if (!slide) {
				break;
			}

			nxt_sq = get_mailbox_num(nxt_sq, directions[i]);
		}
	}

	return 0;
}

// HELPER FUNCTIONS

int Board::in_bounds(int sq) {
	return sq != -1;
}

int Board::is_empty(int sq) {
	return piece[sq] == EMPTY;
}

int Board::diff_colors(int sq1, int sq2) {
	return color[sq1] ^ color[sq2];
}

int Board::get_mailbox_num(int sq, int offset) {
	return mailbox[mailbox64[sq] + offset];
}