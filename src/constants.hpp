#ifndef CONSTANTS_H
#define CONSTANTS_H

#define U64 unsigned long long
#define INF 0x3FFF
#define ENGINE_DEPTH 7

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

enum Square {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
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

const int PIECE_VALUE[7] = {0, 100, 300, 300, 500, 900, INF};

#endif