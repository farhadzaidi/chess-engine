#ifndef BOARD_H
#define BOARD_H

#include <unordered_set>
#include <stack>

class Board {
public:
   
    Board();

    int mailbox64[64] = {
        21, 22, 23, 24, 25, 26, 27, 28,
        31, 32, 33, 34, 35, 36, 37, 38,
        41, 42, 43, 44, 45, 46, 47, 48,
        51, 52, 53, 54, 55, 56, 57, 58,
        61, 62, 63, 64, 65, 66, 67, 68,
        71, 72, 73, 74, 75, 76, 77, 78,
        81, 82, 83, 84, 85, 86, 87, 88,
        91, 92, 93, 94, 95, 96, 97, 98 
    };

    int mailbox[120] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
        -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,   
        -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
        -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
        -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
        -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
        -1, 48, 49, 50, 51, 52, 53, 54, 55, -1, 
        -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  
    };

    // Board representation
    int color[64] = {};
    int piece[64] = {};
    std::unordered_set<int> piece_squares[2];
    std::stack<int> captured_pieces[2];

    // Board attributes loaded from FEN
    int castling_rights = 0;
    int to_move;
    int enpas_sq;
    // Number of halfmoves SINCE last capture or pawn advance 
    // (used for fifty-move rule) 
    int num_plys;
    int num_moves;

    // TODO:
        // Keep track of moves "updated castling rights"
        // Keep track of king square
        // Implement is_attacked

    int make_move(int move);
    int normal(int move);
    int castle(int move);
    int en_passant(int move);
    int promotion(int move);

    void unmake_move(int move);
    void unmake_castle(int move);
    void unmake_en_passant(int move);
    void unmake_promotion(int move);

    int in_bounds(int sq);
    int is_empty(int sq);
    int diff_colors(int sq1, int sq2);
    int get_mailbox_num(int sq, int offset);
    
};

#endif