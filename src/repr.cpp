#include <iostream>
#include <cctype>
#include <unordered_map>

#include "repr.hpp"
#include "constants.hpp"
#include "board.hpp"

static const std::unordered_map<char, int> chr_to_p = {
    std::make_pair('0', EMPTY),
    std::make_pair('p', PAWN),
    std::make_pair('b', BISHOP),
    std::make_pair('n', KNIGHT),
    std::make_pair('r', ROOK),
    std::make_pair('q', QUEEN),
    std::make_pair('k', KING)
};

static const std::unordered_map<int, char> p_to_chr = {
    std::make_pair(EMPTY, '0'),
    std::make_pair(PAWN, 'p'),
    std::make_pair(BISHOP, 'b'),
    std::make_pair(KNIGHT, 'n'),
    std::make_pair(ROOK, 'r'),
    std::make_pair(QUEEN, 'q'),
    std::make_pair(KING, 'k')
};


int chess_to_sq(char rank, char file) {
    int row = '8' - rank;
    int col = file - 97;
    return row * 8 + col;
}

int chess_to_sq(std::string chess) {
    return chess_to_sq(chess[1], chess[0]);
}

std::string sq_to_chess(int sq) {
    int row = sq / 8;
    int col = sq % 8;
    char rank = 8 - row + 48;
    char file = col + 97;

    std::string res;
    res.push_back(file);
    res.push_back(rank);
    return res;
}


// uses FEN notation to setup the board
void load_from_FEN(Board &b, std::string FEN) {
    // Load pieces
    int i = 0;
    int sq = 0;
    while (!std::isspace(FEN[i])) {
        if (std::isalpha(FEN[i])) {
            b.color[sq] = std::isupper(FEN[i]) ? WHITE : BLACK;
            b.piece[sq] = chr_to_p.at(std::tolower(FEN[i]));
            b.piece_squares[b.color[sq]].insert(sq);
            b.material[b.color[sq]] += PIECE_VALUE[b.piece[sq]];
            if (b.piece[sq] == KING) {
                b.king_squares[b.color[sq]] = sq;
            }
            sq++;
        } else if (std::isdigit(FEN[i])) {
            sq += FEN[i] - '0';
        }

        i++;
    }

    // Skip space
    i++;

    // Determine side to move
    b.to_move = FEN[i] == 'w' ? WHITE : BLACK;
    i++;
    
    // Skip space
    i++;

    // Determine castling rights
    if (FEN[i] == '-') {
        i++;
    }
    else {
        while (!std::isspace(FEN[i])) {
            switch (FEN[i]) {
                case 'K':
                    // b.castling_rights[W_SHORT] = 1;
                    b.castling_rights |= W_SHORT;
                    break;
                case 'Q':
                    b.castling_rights |= W_LONG;
                    break;
                case 'k':
                    b.castling_rights |= B_SHORT;
                    break;
                case 'q':
                    b.castling_rights |= B_LONG;
                    break;
            }

            i++;
        }
    }

    // Skip space
    i++;

    // Determine en passant square
    if (FEN[i] == '-') {
        b.enpas_sq.push(-1);
        i++;
    }
    else {
        b.enpas_sq.push(chess_to_sq(FEN[i + 1], FEN[i]));
        i += 2;
    }

    // Skip space
    i++;

    // Get number of half moves and full moves
    b.num_plys = FEN[i] - '0';
    i += 2;

    std::string num_moves = "";
    while (!std::isspace(FEN[i])) {
        num_moves += FEN[i];
        i++;
    }
    b.num_moves = std::stoi(num_moves);

    b.set_zobrist_hash();
}


void print_board(Board &b) {
    std::cout << "\n\n";
    for (int row = 0; row < 8; row++) {
        std::cout << "\t" << 8 - row << "   ";
        for (int col = 0; col < 8; col++) {
            int sq = row * 8 + col;
            char symbol = p_to_chr.at(b.piece[sq]);
            if (b.color[sq] == WHITE) {
                symbol = std::toupper(symbol);
            }
            std::cout << symbol << "  ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\t    a  b  c  d  e  f  g  h\n\n";
}


void print_attr(Board &b) {
    std::cout << "\n\n";

    // Print side to move
    char to_move = b.to_move ? 'w' : 'b';
    std::cout << "Side to move: " << to_move << "\n";

    // Print castling rights
    std::cout << "Castling rights:";
    if (b.castling_rights & B_LONG) {
        std::cout << " b(O-O-O),";
    }

    if (b.castling_rights & B_SHORT) {
        std::cout << " b(O-O),";
    }

    if (b.castling_rights & W_LONG) {
        std::cout << " w(O-O-O),";
    }

    if (b.castling_rights & W_SHORT) {
        std::cout << " w(O-O)";
    }

    std::cout << "\n";

    // Print en passant target square
    if (b.enpas_sq.top() == -1) {
        std::cout << "No en passant target square";
    } else {
        std::string ep_sq_chess = sq_to_chess(b.enpas_sq.top());
        std::cout << "En passant target square: " << ep_sq_chess;
    }

    std::cout << "\n";

    // Print pieces
    std::cout << "Black pieces: ";
    for (int sq : b.piece_squares[0]) {
        std::cout << p_to_chr.at(b.piece[sq]) << ", ";
    }

    std::cout << "\nWhite pieces: ";
    for (int sq : b.piece_squares[1]) {
        std::cout << p_to_chr.at(b.piece[sq]) << ", ";
    }

    std::cout << "\n\n";
}