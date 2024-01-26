#ifndef EVALUATION_H
#define EVALUATION_H

class Board;

// Once the total number of pieces on the board is at or below this value,
// the game has entered the endgame and we can change the eval function to
// alter the engine's behavior in the endgame
const int ENDGAME_THRESHOLD = 16;

const int PAWN_EVAL_MAP[2][64] = {
	// WHITE
	{
		0,  0,  0,  0,  0,  0,  0,  0,
		5,  5,  5,  5,  5,  5,  5,  5,
		4,  4,  4,  4,  4,  4,  4,  4,
		3,  3,  3,  4,  4,  3,  3,  3,
		2,  2,  2,  3,  3,  2,  2,  2,
		1,  1,  1,  1,  1,  1,  1,  1,
		0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,
	},

	// BLACK
	{
		0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,
		1,  1,  1,  1,  1,  1,  1,  1,
		2,  2,  2,  3,  3,  2,  2,  2,
		3,  3,  3,  4,  4,  3,  3,  3,
		4,  4,  4,  4,  4,  4,  4,  4,
		5,  5,  5,  5,  5,  5,  5,  5,
		0,  0,  0,  0,  0,  0,  0,  0,
	}
};

// Emphasize placing knights in the middle to control more squares

const int KNIGHT_EVAL_MAP[64] = {
	 0,  0,  0,  0,  0,  0,  0,  0,
	 0,  1,  1,  1,  1,  1,  1,  0,
	 0,  1,  3,  3,  3,  3,  1,  0,
	 0,  1,  3,  4,  4,  3,  1,  0,
	 0,  1,  3,  4,  4,  3,  1,  0,
	 0,  1,  3,  3,  3,  3,  1,  0,
	 0,  1,  1,  1,  1,  1,  1,  0,
	 0,  0,  0,  0,  0,  0,  0,  0, 
};

// Focus on bringing bishops to middle to control central diagonals

const int BISHOP_EVAL_MAP[2][64] = {
	// LIGHT SQUARED BISHOP
	{
		0,  0,  0,  0,  0,  0,  0,  0,
		0,  1,  0,  1,  0,  1,  0,  0,
		0,  0,  3,  0,  3,  0,  1,  0,
		0,  1,  0,  4,  0,  3,  0,  0,
		0,  0,  3,  0,  4,  0,  1,  0,
		0,  1,  0,  3,  0,  3,  0,  0,
		0,  0,  1,  0,  1,  0,  1,  0,
		0,  0,  0,  0,  0,  0,  0,  0,
	},
	// DARK SQUARED BISHOP
	{
		0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  1,  0,  1,  0,  1,  0,
		0,  1,  0,  3,  0,  3,  0,  0,
		0,  0,  3,  0,  4,  0,  1,  0,
		0,  1,  0,  4,  0,  3,  0,  0,
		0,  0,  3,  0,  3,  0,  1,  0,
		0,  1,  0,  1,  0,  1,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,
	}
};

const int KING_EVAL_MAP[2][64] = {
	// WHITE
	{
		-4, -4, -4, -4, -4, -4, -4, -4,
		-4, -4, -4, -4, -4, -4, -4, -4,
		-4, -4, -4, -4, -4, -4, -4, -4,
		-3, -3, -3, -3, -3, -3, -3, -3,
		-1, -1, -2, -2, -2, -2, -1, -1,
		 0,  0, -1, -1, -1, -1,  0,  0,
		 4,  2,  1,  0,  0,  1,  2,  4,
		10, 10,  8,  6,  6,  8, 10, 10,
	},

	// BLACK
	{
		10, 10,  8,  6,  6,  8, 10, 10,
		 4,  2,  1,  0,  0,  1,  2,  4,
		 0,  0, -1, -1, -1, -1,  0,  0,
		-1, -1, -2, -2, -2, -2, -1, -1,
		-3, -3, -3, -3, -3, -3, -3, -3,
		-4, -4, -4, -4, -4, -4, -4, -4,
		-4, -4, -4, -4, -4, -4, -4, -4,
		-4, -4, -4, -4, -4, -4, -4, -4,
	}
};

int eval(Board &b, int game_over = 0);

#endif