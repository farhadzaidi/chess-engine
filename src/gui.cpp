#include <vector>

#include "constants.hpp"
#include "gui.hpp"
#include "board.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "search.hpp"
#include "event.hpp"
#include "draw.hpp"

void run_gui(Board &b) {
    // Initialize window
    sf::RenderWindow window(sf::VideoMode(1500, 1500), "Chess");
    sf::Font font;
    font.loadFromFile("assets/fonts/DejaVuSans.ttf");

	int player_side = WHITE;
	int from = -1;
	int to = -1;
    std::vector<int> valid_moves = validate_moves(b, gen_moves(b));

    // Main loop
    while (window.isOpen()) {
    	// Handle input from player
        sf::Event event;
        while (window.pollEvent(event)) {
        	handle_close(window, event);
            // Move piece
        	handle_leftclick(window, event, b, from, to, player_side, valid_moves);
            // Undo move
        	handle_leftarrow(event, b, valid_moves);
        }

        // Draw graphics
    	window.clear(LIGHT);
        draw_board(window, font);
        draw_pieces(window, b);
        draw_moves(window, b, from, valid_moves);
        window.display();


        if (!b.game_over()) {
            // Generate and make engine move
            if (b.to_move != player_side) {
                int move = search(b, ENGINE_DEPTH);
                b.make_move(move);
                valid_moves = validate_moves(b, gen_moves(b));
            }
        }
    }
}