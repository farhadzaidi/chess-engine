
/*

This file defines a number of constants and enums used throughout the entire
project. These definitions are vital for the program to function as they define
various entities such as piece types, piece color, move flags, etc.

The decision to represent various entities throughout the project as integers
(as opposed to strings or objects) is made in an effort to speed up and optimize
the computations being made during move generation and search.

*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define U64 unsigned long long
#define INF 0x3FFF

enum Color {
    BLACK,
    WHITE
};

enum Piece {
    EMPTY,
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
};

enum MoveType {
	QUIET,
	CAPTURE
};

enum MoveFlag {
	NORMAL,
	EN_PASSANT,
	CASTLE,
	P_BISHOP,
	P_KNIGHT,
	P_ROOK,
	P_QUEEN
};

enum CastlingRights {
    B_SHORT = 1,
    B_LONG = 2,
    W_SHORT = 4,
    W_LONG = 8
};

enum ttNodeType {
    EXACT,
    ALPHA,
    BETA
};

const int BISHOP_MOVES[8] = {-11, -9, 9, 11, 0, 0, 0, 0};
const int KNIGHT_MOVES[8] = {-21, -19, -12, -8, 8, 12, 19, 21};
const int ROOK_MOVES[8] = {-10, -1, 1, 10, 0, 0, 0, 0};
const int KING_QUEEN_MOVES[8] = {-11, -10, -9, -1, 1, 9, 10, 11};

// Starting squares for left and right rooks on both sides (useful for castling)
const int WLR_START = a1;
const int WRR_START = h1;
const int BLR_START = a8;
const int BRR_START = h8;

// Material values for each piece type
const int PIECE_VALUE[7] = {0, 100, 300, 300, 500, 900, INF};

#endif