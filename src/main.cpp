#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <vector>
#include <cmath>
#include <chrono>

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

void draw_moves(sf::RenderWindow &window, Board &b, int from,
	std::vector<int> valid_moves) {
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
		c.setRadius(20);
		c.setPosition(150 * (col + 1) + 50, 150 * (row + 1) + 50);
		c.setFillColor(INFO);
		window.draw(c);
	}
}

std::vector<int> validate_moves(Board &b, std::vector<int> moves) {
	std::vector<int> valid_moves;
	for (int move : moves) {
		if (b.make_move(move)) {
			valid_moves.push_back(move);
		}

		b.unmake_move(move);
	}

	return valid_moves;
}

void print_move(int move) {
	int from = get_from(move);
	int to  = get_to(move);

	std::cout << sq_to_chess(from) << sq_to_chess(to) << "\n";
}

int main(int argc, char* argv[]) {
	// Initialize board
	std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Board b;
	load_from_FEN(b, FEN);

	// Perft
	if (argc > 2 && std::string(argv[1]).compare("perft") == 0) {
		std::cout << "\nMove Generation Performance Test\n";
		int depth = std::stoi(argv[2]);

		auto start = std::chrono::high_resolution_clock::now();
		int nodes = perft(b, depth);
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration<double>(end - start);
		
		int expected_nodes = perft_expected[depth];
		std::cout << nodes << " nodes generated at depth " << depth
			<< " in " << elapsed.count() << " seconds\n";

		if (nodes == expected_nodes) {
			std::cout << "Expected result\n";
		} else {
			int diff = abs(nodes - expected_nodes);
			std::cout << "Unexpected result (off by " << diff << ")\n";
			std::cout << "Expected result: " << expected_nodes << "\n";
		}

		return 0;
	}

	// int engine_side = BLACK;
	// int player_side = WHITE;

	sf::RenderWindow window(sf::VideoMode(1500, 1500), "Chess");
    sf::Font font;
	font.loadFromFile("assets/fonts/DejaVuSans.ttf");

	std::cout << "\n\n------------------------------------------\n";
	std::cout << "----------\tRUNNING GUI\t----------\n";
	std::cout << "------------------------------------------\n\n";

	// print_board(b);
	// print_attr(b);

	std::vector<int> valid_moves = validate_moves(b, gen_moves(b));
	int from = -1;
	int to = -1;
	std::stack<int> move_list;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
        	// Close
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
						// Only set "from" if the selected piece is on the
						// side to move
						if (b.piece[sq] != EMPTY && b.color[sq] == b.to_move) {
							from = sq;
						}
            		} else if (to == -1) {
	            		to = row * 8 + col;
	            		// Make sure "to" points to an empty square or enemy piece
	            		if (b.piece[to] == EMPTY || b.color[to] != b.to_move) {
	            			// Find the correct move from valid_moves and make it
	            			for (int move : valid_moves) {
	            				int move_from = get_from(move);
	            				int move_to = get_to(move);
	            				if (from == move_from && to == move_to) {
			            			move_list.push(move);
			            			b.make_move(move);
			            			print_move(move);
			            			// print_board(b);
			            			// print_attr(b);

			            			// Regenerate valid moves after a successful
			            			// move
	            					valid_moves = validate_moves(b, gen_moves(b));
			            			break;
	            				}
	            			}

	            			// Reset "from" and "to" regardless of if the move
	            			// was made
	            			from = -1;
	            			to = -1;
	            		} else {
	            			// If another friendly piece is selected, set "from"
	            			// to that piece and reset "to"
	            			// Makes piece selection more seamless
	            			from = to;
	            			to = -1;
	            		}
            		}
        		}
            }

            // Left arrow key (undo)
            if (event.type == sf::Event::KeyPressed
            	&& event.key.code == sf::Keyboard::Left) {
            	if (!move_list.empty()) {
            		int move = move_list.top();
            		move_list.pop();
            		b.unmake_move(move);
            		std::cout << "undo ";
            		print_move(move);
             		// print_board(b);
			        // print_attr(b);

			        // Regenerate valid moves after undo
			        valid_moves = validate_moves(b, gen_moves(b));
            	}
            }
        }

    	window.clear(LIGHT);
        draw_board(window, font);
        draw_pieces(window, b);
        draw_moves(window, b, from, valid_moves);
        window.display();
    }

    return 0;
}


