#ifndef BOARD_H
#define BOARD_H

#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <random>

#include "constants.hpp"

/*

BOARD REPRESENTATION:

The board is represented by two arrays of 64 elements, one for the piece type
and one for the color of the piece. We utilize a set of mailbox boards to make
it easy to check if a piece is out-of-bounds when moving pieces during move
generation. 

Additionally, we use a technique called Zobrist hashing to hash each board
position. These hashes serve as entries in the transposition table which can be
used to determine if we have seen a position before.

*/


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

    // Random number generator for zobrist hashes
    std::mt19937_64 rng;

    // Board representation
    int color[64] = {};
    int piece[64] = {};
    std::unordered_set<int> piece_squares[2];
    std::stack<int> captured_pieces[2];

    // Board attributes loaded from FEN
    int castling_rights = 0;
    int to_move;
    std::stack<int> enpas_sq;

    // Other useful attributes
    std::stack<int> castling_rights_updates;
    int king_squares[2];
    std::stack<int> move_list;
    int material[2] = {};

    // Stores Zobrist hashes (randomly generated at board initialization) for
    // each combination of piece color, type, and position
    // We also have Zobrist hashes for other board attributes
    // This stuff is used to hash positions for the transposition table
    U64 zobrist_piece_table[2][7][64];
    U64 zobrist_enpas_table[8];
    U64 zobrist_castle_table[16];
    U64 zobrist_to_move_hash;
    U64 zobrist_hash;
    std::unordered_map<U64, int> trans_table; 

    void initialize_zobrist_tables();

    int make_move(int move, int real_move = 0);
    void unmake_move(int move, int real_move = 0);
    void update_castling_rights(int moving_piece);
    int is_attacked(int sq);
    int game_over();
    void set_zobrist_hash();

    int in_bounds(int sq);
    int is_empty(int sq);
    int diff_colors(int sq1, int sq2);
    int get_mailbox_num(int sq, int offset);
    U64 gen_rand_U64();
    
};

#endif