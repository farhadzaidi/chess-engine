#include "board.hpp"
#include "constants.hpp"
#include "move.hpp"

#include <iostream>
#include <algorithm>

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
	// Castle
	// En Passant
	// Promotion
	// Move Validation

	int from = get_from(move);
	int to = get_to(move);
	int mtype = get_mtype(move);
	// int flag = get_flag(move);

	// Normal move
	if (mtype == CAPTURE) {
		piece_squares[!to_move].erase(to);
		captured_pieces[to_move].push(piece[to]);
	}

	piece_squares[to_move].erase(from);
	piece_squares[to_move].insert(to);
	piece[to] = piece[from];
	color[to] = color[from];
	piece[from] = EMPTY;
	color[from] = EMPTY;

	// Switch turn
	to_move = !to_move;

	return 1;
}

int Board::unmake_move(int move) {
	int from = get_from(move);
	int to = get_to(move);
	int mtype = get_mtype(move);
	// int flag = get_flag(move);

	// Revert turn
	to_move = !to_move;

	// Unmake normal move
	piece[from] = piece[to];
	color[from] = color[to];
	piece_squares[to_move].erase(to);
	piece_squares[to_move].insert(from);

	if (mtype == CAPTURE) {
		piece_squares[!to_move].insert(to);
		int captured_piece = captured_pieces[to_move].top();
		captured_pieces[to_move].pop();
		piece[to] = captured_piece;
		color[to] = !to_move;
	} else {
		piece[to] = EMPTY;
		color[to] = EMPTY;
	}

	return 1;
}
