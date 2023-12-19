#include "board.h"
#include "constants.h"

Board::Board() {}

int get_mailbox_num(int sq, int offset) {
	return mailbox[mailbox64[sq] + offset];
}

int in_bounds(int sq) {
	return sq != -1;
}

int is_empty(int sq) {
	return piece[sq] == EMPTY;
}

int diff_colors(int sq1, int sq2) {
	return color[sq1] ^ color[sq2];
}