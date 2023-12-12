#include <iostream>

#include "constants.h"
#include "move.h"
#include "board.h"
#include "repr.h"

int main() {
    std::string start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    // std::string start_pos = "8/8/n2P4/p7/8/8/8/8 w KQkq - 0 1";
    // std::string start_pos = "8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 50";
    Board b;
    load_from_FEN(b, start_pos);
    print_board(b);
    print_attr(b);

    return 0;
}