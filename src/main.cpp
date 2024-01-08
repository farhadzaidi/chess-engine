#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <vector>

#include "SFML/Graphics.hpp"
#include "constants.hpp"
#include "board.hpp"
#include "repr.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "perft.hpp"

sf::Color LIGHT(245, 245, 245);
sf::Color DARK(46, 46, 56);
sf::Color INFO(127, 212, 245);

void draw_board(sf::RenderWindow &window, sf::Font &font) {
	// Draw border
	sf::RectangleShape bd;
	bd.setSize(sf::Vector2f(1200, 1200));
	bd.setPosition(150, 150);
	bd.setFillColor(sf::Color::Transparent);
	bd.setOutlineColor(DARK);
	bd.setOutlineThickness(5);
	window.draw(bd);

	// Draw 8x8 chessboard pattern
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			sf::RectangleShape sq;
			sq.setSize(sf::Vector2f(150, 150));
			sq.setPosition(150 * (r + 1), 150 * (c + 1));
			sq.setFillColor((r + c) & 1 ? DARK : LIGHT);
			window.draw(sq);
		}
	}

	// Draw ranks
	for (int i = 8; i >= 1; i--) {
		sf::Text rank;
		rank.setString(std::to_string(i));
		rank.setFont(font);
		rank.setCharacterSize(80);
		rank.setPosition(40, 150 * (8 - i) + 170);
		rank.setFillColor(DARK);
		window.draw(rank);

		sf::Text file;
		char file_chr = 'h' - i + 1;
		file.setString(file_chr);
		file.setFont(font);
		file.setCharacterSize(80);
		file.setPosition(150 * (8 - i) + 200, 1370);
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
		spr.setPosition(150 * (col + 1) + 10, 150 * (row + 1) + 10);
		window.draw(spr);
	}
}

void draw_moves(sf::RenderWindow &window, Board &b, int from) {
	if (from == -1)
		return;

	std::vector<int> moves;
	gen_moves(b, from, moves);
	for (int move : moves) {
		int valid = b.make_move(move);
		b.unmake_move(move);
		if (!valid) {
			continue;
		}

		int to = get_to(move);
		int row = to / 8;
		int col = to % 8;

		sf::CircleShape c;
		c.setRadius(20);
		c.setPosition(150 * (col + 1) + 50, 150 * (row + 1) + 50);
		c.setFillColor(INFO);
		window.draw(c);
	}
}

int main(int argc, char* argv[]) {
	std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Board b;
	load_from_FEN(b, FEN);

	if (argc > 2 && std::string(argv[1]).compare("perft") == 0) {
		int depth = std::stoi(argv[2]);
		std::cout << perft(depth, b, depth) << "\n";
		return 0;
	}

	print_board(b);
	print_attr(b);

	sf::RenderWindow window(sf::VideoMode(1500, 1500), "Chess");
    sf::Font font;
	font.loadFromFile("assets/fonts/DejaVuSans.ttf");

	int from = -1;
	int to = -1;
	std::stack<int> moves;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Left click
            if (event.type == sf::Event::MouseButtonPressed
            	&& event.mouseButton.button == sf::Mouse::Left) {
            	// Get position of click and compute row/col
            	sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        		int col = mouse_pos.x / 150 - 1;
        		int row = mouse_pos.y / 150 - 1;

        		// Ensure the click area is in bounds before processing further
        		if (0 <= col && col <= 7 && 0 <= row && row <= 7) {
					if (from == -1 && to == -1) {
						int sq = row * 8 + col;
						if (b.piece[sq] != EMPTY && b.color[sq] == b.to_move) {
							from = sq;
						}
            		} else if (to == -1) {
	            		to = row * 8 + col;
	            		if (b.piece[to] == EMPTY || b.color[to] != b.to_move) {
	            			// TODO: fix this
	            			std::vector<int> from_moves;
	            			gen_moves(b, from, from_moves);
	            			for (int from_move : from_moves) {
	            				int move_to = get_to(from_move);
	            				if (to == move_to) {
			            			moves.push(from_move);
			            			int valid = b.make_move(from_move);

			            			if (!valid) {
			            				b.unmake_move(from_move);
			            			} else {
				            			print_board(b);
				            			print_attr(b);
			            			}

			            			break;
	            				}
	            			}

	            			from = -1;
	            			to = -1;
	            		} else {
	            			from = to;
	            			to = -1;
	            		}
            		}
        		}
            }

            // Left arrow key (undo)
            if (event.type == sf::Event::KeyPressed
            	&& event.key.code == sf::Keyboard::Left) {
            	if (!moves.empty()) {
            		int move = moves.top();
            		moves.pop();
            		b.unmake_move(move);
            		print_board(b);
			        print_attr(b);
            	}
            }
        }

    	window.clear(LIGHT);
        draw_board(window, font);
        draw_pieces(window, b);
        draw_moves(window, b, from);
        window.display();
    }

    return 0;
}


