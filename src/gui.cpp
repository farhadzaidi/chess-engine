#include <vector>

#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "gui.hpp"
#include "board.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "search.hpp"

#include <iostream>

const int WINDOW_SIZE = 1500;
const float u = WINDOW_SIZE / 10;

const sf::Color LIGHT(245, 245, 245);
const sf::Color DARK(46, 46, 56);
const sf::Color INFO(127, 212, 245);

void draw_board(sf::RenderWindow &window, sf::Font &font) {
    // Draw border
    sf::RectangleShape bd;
    bd.setSize(sf::Vector2f(8*u, 8*u));
    bd.setPosition(u, u);
    bd.setFillColor(sf::Color::Transparent);
    bd.setOutlineColor(DARK);
    bd.setOutlineThickness(5);
    window.draw(bd);

    // Draw 8x8 chessboard pattern
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            sf::RectangleShape sq;
            sq.setSize(sf::Vector2f(u, u));
            sq.setPosition(u * (r + 1), u * (c + 1));
            sq.setFillColor((r + c) & 1 ? DARK : LIGHT);
            window.draw(sq);
        }
    }

    // Draw ranks and files
    for (int i = 8; i >= 1; i--) {
        sf::Text rank;
        rank.setString(std::to_string(i));
        rank.setFont(font);
        rank.setCharacterSize(0.5*u);
        rank.setPosition(0.35*u, u * (8 - i) + 1.2*u);
        rank.setFillColor(DARK);
        window.draw(rank);

        sf::Text file;
        char file_chr = 'h' - i + 1;
        file.setString(file_chr);
        file.setFont(font);
        file.setCharacterSize(0.5*u);
        file.setPosition(u * (8 - i) + 1.3*u, 9.1*u);
        file.setFillColor(DARK);
        window.draw(file);
    }
}

void draw_pieces(sf::RenderWindow &window, Board &b) {
    std::unordered_map<int, std::string> color_map = {
        std::make_pair(BLACK, "b"),
        std::make_pair(WHITE, "w")
    };
    std::unordered_map<int, std::string> piece_map = {
        std::make_pair(PAWN, "p"),
        std::make_pair(BISHOP, "b"),
        std::make_pair(KNIGHT, "n"),
        std::make_pair(ROOK, "r"),
        std::make_pair(QUEEN, "q"),
        std::make_pair(KING, "k"),
    };

    for (int i = 0; i < 64; i++) {
        if (b.piece[i] == EMPTY) {
            continue;
        }

        sf::Image img;
        img.loadFromFile(
            "assets/images/"
            + color_map.at(b.color[i])
            + piece_map.at(b.piece[i]) 
            + ".png"
        );
        sf::Texture texture;
        texture.loadFromImage(img);
        sf::Sprite spr(texture);

        int row = i / 8;
        int col = i % 8;
        spr.setScale(0.007*u, 0.007*u);
        spr.setPosition(u * (col + 1) + 0.05*u, u * (row + 1) + 0.05*u);
        window.draw(spr);
    }
}

void draw_moves(
    sf::RenderWindow &window, 
    Board &b, 
    int from,
    std::vector<int> valid_moves
) {
    if (from == -1)
        return;

    // Draw only valid moves where the move from matches "from"
    for (int move : valid_moves) {
        int move_from = get_from(move);
        if (from != move_from) {
            continue;
        }

        int to = get_to(move);
        int row = to / 8;
        int col = to % 8;

        sf::CircleShape c;
        c.setRadius(0.15*u);
        c.setPosition(u * (col + 1) + 0.35*u, u * (row + 1) + 0.35*u);
        c.setFillColor(INFO);
        window.draw(c);
    }
}


void run_gui(Board &b) {
    // Initialize window
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), 
        "Chess", 
        sf::Style::Default, 
        sf::ContextSettings(0, 0, 8)
    );
    sf::Font font;
    font.loadFromFile("assets/fonts/DejaVuSans.ttf");

	int player_side = WHITE;
	int from = -1;
	int to = -1;
    std::vector<int> valid_moves = validate_moves(b, gen_moves(b));

    while (window.isOpen()) {
    	// Handle input from player
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (!b.game_over()) {
                // Handle player move
                if (event.type == sf::Event::MouseButtonPressed 
                    && event.mouseButton.button == sf::Mouse::Left) {
                    // Get position of click and compute row/col
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    int col = mouse_pos.x / u - 1;
                    int row = mouse_pos.y / u - 1;

                    // Ensure the click area is in bounds before processing further
                    if (0 <= col && col <= 7 && 0 <= row && row <= 7) {
                        if (from == -1 && to == -1) {
                            int sq = row * 8 + col;
                            // Only set "from" if the selected piece is on the
                            // side to move and it's the player's turn
                            if (b.piece[sq] != EMPTY && b.color[sq] == player_side
                                && b.to_move == player_side) {
                                from = sq;
                            }
                        } else if (to == -1) {
                            to = row * 8 + col;
                            // Make sure "to" points to an empty square or enemy piece
                            if (b.piece[to] == EMPTY || b.color[to] != player_side) {
                                // Find the correct move from valid_moves and make it
                                for (int move : valid_moves) {
                                    if (from == get_from(move) && to == get_to(move)) {
                                        b.make_move(move);
                                        valid_moves = validate_moves(b, gen_moves(b));
                                        break;
                                    }
                                }

                                // Reset "from" and "to"
                                from = -1;
                                to = -1;
                            } else {
                                // Another friendly piece selected
                                from = to;
                                to = -1;
                            }
                        }
                    }
                }
            }

            // Undo move
            if (event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Left) {
                if (!b.move_list.empty()) {
                    b.unmake_move(b.move_list.top()); // unmake engine move
                    int player_move = b.move_list.top();
                    b.unmake_move(player_move);
                    // Regenerate valid moves after undo
                    valid_moves = validate_moves(b, gen_moves(b));
                }
            }
        }

        // Draw graphics
        window.clear(LIGHT);
        draw_board(window, font);
        draw_pieces(window, b);
        draw_moves(window, b, from, valid_moves);
        window.display();

        // Handle engine move
        if (!b.game_over() && b.to_move != player_side) {
            // int move = search(b, 6);
            int move = iterative_search(b);
            b.make_move(move);
            valid_moves = validate_moves(b, gen_moves(b));
        }
    }
}