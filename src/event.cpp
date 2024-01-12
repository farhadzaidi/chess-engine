#include <string>

#include "SFML/Graphics.hpp"
#include "constants.hpp"
#include "event.hpp"
#include "move.hpp"
#include "movegen.hpp"

void handle_close(sf::RenderWindow &window, sf::Event &event) {
	if (event.type == sf::Event::Closed) {
	    window.close();
	}
}

void handle_leftclick(
	sf::RenderWindow &window,
	sf::Event &event,
	Board &b,
	int &from,
	int &to,
	int player_side,
	std::vector<int> &valid_moves
) {
	if (b.game_over()) {
		return;
	}

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

void handle_leftarrow(
	sf::Event &event,
	Board &b,
	std::vector<int> &valid_moves
) {
    if (event.type == sf::Event::KeyPressed
    	&& event.key.code == sf::Keyboard::Left) {
    	if (!b.move_list.empty()) {
    		b.unmake_move(b.move_list.top()); // unmake engine move
    		int player_move = b.move_list.top();
    		b.unmake_move(player_move);
    	}

        // Regenerate valid moves after undo
        valid_moves = validate_moves(b, gen_moves(b));
    }
}