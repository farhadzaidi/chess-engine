#include "board.hpp"

const unsigned long long perft_expected[8] = {
	1,
	20,
	400,
	8'902,
	197'281,
	4'865'609,
	119'060'324,
	3'195'901'860
};

unsigned long long perft(Board &b, int depth);