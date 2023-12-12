#include "move.h"

int Move::new_move(int from, int to, int mtype, int flag) {
	to <<= 6;
	mtype <<= 12;
	flag <<= 13;
	return from | to | mtype | flag;
}

int get_from(int m) {
	return m & 63;
}

int get_to(int m) {
	return (m >> 6) & 63;
}

int get_mtype(int m) {
	return (m >> 12) & 1;
}

int get_flag(int m) {
	return (m >> 13) & 7;
}