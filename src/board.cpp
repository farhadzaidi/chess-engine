#include <cmath>

#include "board.hpp"
#include "constants.hpp"
#include "move.hpp"
#include "movegen.hpp"

#include <iostream>

Board::Board() {}

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
		// Handle capture logic slightly differently for en passant
			// Captured piece is south of "to"
			// Captured piece square needs to be emptied 
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

	// Adjust moving piece square in piece_squares
	piece_squares[to_move].erase(from);
	piece_squares[to_move].insert(to);

	// Move piece to "to" and empty "from"
	piece[to] = piece[from];
	color[to] = color[from];
	piece[from] = EMPTY;
	color[from] = EMPTY;

	// For castling, simply move rook to right or left of king based
	// on whether the move is a short or long castle
	// Also, empty rook's old position and adjust piece_squares
	// for the moving rook
	if (flag == CASTLE) {
		valid &= !is_attacked(from);

		int new_rook_pos;
		int cur_rook_pos;
		// Long castle
		if (from > to) {
			valid &= !is_attacked(from - 1);
			new_rook_pos = to + 1;
			cur_rook_pos = to_move == WHITE ? WLR_START : BLR_START;
		} else { // Short castle
			valid &= !is_attacked(from + 1);
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

	update_castling_rights(piece[to]);

	if (piece[to] == KING) {
		king_squares[to_move] = to;
	}

	// Validate move
	valid &= !is_attacked(king_squares[to_move]);

	// Switch turn
	to_move = !to_move;

	// Add to move list
	move_list.push(move);

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
		// Capture logic is mostly same for normal captures and en passant,
		// just change the capture square
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
		// and adjust piece_squares
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

	// Pop the latest castling rights update and revert it
	int castling_rights_update = castling_rights_updates.top();
	castling_rights_updates.pop();
	castling_rights |= castling_rights_update;

	if (piece[from] == KING) {
		king_squares[to_move] = from;
	}

	// Remove from move list
	move_list.pop();
}

void Board::update_castling_rights(int moving_piece) {
	int update = 0;
	if (castling_rights) {
		int is_white = to_move == WHITE;
		int short_castling_right = is_white ? W_SHORT : B_SHORT;
		int long_castling_right = is_white ? W_LONG : B_LONG;
		int right_rook_start = is_white ? WRR_START : BRR_START;
		int left_rook_start = is_white ? WLR_START : BLR_START;

		if (moving_piece == KING) {
			if (castling_rights & short_castling_right) {
				castling_rights ^= short_castling_right;
				update |= short_castling_right;
			}

			if (castling_rights & long_castling_right) {
				castling_rights ^= long_castling_right;
				update |= long_castling_right;
			}
		}

		if (moving_piece == ROOK) {
			if (castling_rights & short_castling_right 
					&& is_empty(right_rook_start)) {
				castling_rights ^= short_castling_right;
				update |= short_castling_right;
			}

			if (castling_rights & long_castling_right 
					&& is_empty(left_rook_start)) {
				castling_rights ^= long_castling_right;
				update |= long_castling_right;
			}	
		}
	}
	castling_rights_updates.push(update);
}

int Board::is_attacked(int sq) {
	int slide[5] = {0, 1, 0, 1, 0};
	int attackers[5][2] = {
	    {PAWN, 0},
	    {BISHOP, QUEEN},
	    {KNIGHT, 0},
	    {ROOK, QUEEN},
	    {KING, 0}
	};
	int attacks[5][8] = {
	    {11, 9, 0, 0, 0, 0, 0, 0},
	    {-11, -9, 9, 11, 0, 0, 0, 0},
	    {-21, -19, -12, -8, 8, 12, 19, 21},
	    {-10, -1, 1, 10, 0, 0, 0, 0},
	    {-11, -10, -9, -1, 1, 9, 10, 11}
	};

	// Adjust pawn attacks based on perspective
	if (to_move == WHITE) {
		attacks[0][0] *= -1;
		attacks[0][1] *= -1;
	}

	// Outer loop is used to iterate through the different attack types
	for (int i = 0; i < 5; i++) {
		// Inner loop goes through each individual direction the current
		// attack type
		for (int j = 0; j < 8; j++) {
			// Not all piece types have 8 attacks so break early
			if (attacks[i][j] == 0) {
				break;
			}

			int nxt_sq = get_mailbox_num(sq, attacks[i][j]);
			while (in_bounds(nxt_sq)) {
				// If the next square is not empty, then we have to see if it
				// is an attacker of the current attack type (e.g. if we
				// are looping through diagnal attacks then we have to see
				// if the attacker is a bishop or a queen)
				if (!is_empty(nxt_sq)) {
					if (color[nxt_sq] != to_move) {
						for (int k = 0; k < 2; k++) {
							if (piece[nxt_sq] == attackers[i][k]) {
								return 1;
							}
						}
					}

					break;
				}

				// No need to keep looping for nonsliding pieces
				if (!slide[i]) {
					break;
				}

				nxt_sq = get_mailbox_num(nxt_sq, attacks[i][j]);
			}
		}
	}

	return 0;
}

int Board::game_over() {
	std::vector<int> moves = gen_moves(*this);
	for (int move : moves) {
		if (make_move(move)) {
			unmake_move(move);
			return 0;
		}

		unmake_move(move);
	}

	return 1;
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