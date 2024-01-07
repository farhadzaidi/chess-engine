#include "board.hpp"
#include "constants.hpp"
#include "move.hpp"

#include <iostream>
#include <algorithm>

// NOTES:
	// A fair amount of branching in make_move and unmake_move since I am trying
	// to keep all the logic limited to one function each
	// I could potentially see performance improvements by creating separate
	// functions for each move type (normal, en passant, castle, promotion) and
	// using a switch statement for the flag.
	// More code, but it could be faster and (maybe) easier to understand

Board::Board() {}

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

int Board::make_move(int move) {
	// TODO:
		// update castling rights
		// update king square
		// update en passant target square
		// validate castle using is_attacked
			// check if "king_sq" is attacked
			// (final king_sq pos gets checked at the end)
			// for short castle, check if from + 1 is attacked
			// for long castle, check if from - 1 is attacked
	int from = get_from(move);
	int to = get_to(move);
	int mtype = get_mtype(move);
	int flag = get_flag(move);
	int valid = 1;

	if (mtype == CAPTURE) {
		// Change square of captured piece to south of "to" if the move type is
		// en passant
		int south = to_move == WHITE ? 8 : -8;
		int cap_sq = flag == EN_PASSANT ? to + south : to;

		piece_squares[!to_move].erase(cap_sq);
		captured_pieces[to_move].push(piece[cap_sq]);
	}

	piece_squares[to_move].erase(from);
	piece_squares[to_move].insert(to);

	piece[to] = piece[from];
	color[to] = color[from];
	piece[from] = EMPTY;
	color[from] = EMPTY;

	if (flag == CASTLE) {
		int new_rook_pos;
		int cur_rook_pos;
		// Long castle
		if (from > to) {
			new_rook_pos = to + 1;
			cur_rook_pos = to_move == WHITE ? WLR_START : BLR_START;
		} else { // Short castle
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

	// Switch turn
	to_move = !to_move;

	// TODO: Validate move using is_attacked
	return 1;
}

void Board::unmake_move(int move) {
	// TODO:
		// undo castling rights update
		// undo king square update
		// undo ep target square update
	int from = get_from(move);
	int to = get_to(move);
	int mtype = get_mtype(move);
	int flag = get_flag(move);

	// Revert turn
	to_move = !to_move;

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
}

// TODO: implement is_attacked(int sq)