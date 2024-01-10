#ifndef EVENT_H
#define EVENT_H

#include <vector>

#include "SFML/Graphics.hpp"
#include "board.hpp"

void handle_close(sf::RenderWindow &window, sf::Event &event);
void handle_leftclick(
	sf::RenderWindow &window,
	sf::Event &event,
	Board &b,
	int &from,
	int &to,
	int player_side,
	std::vector<int> &valid_moves
);
void handle_leftarrow(
	sf::Event &event,
	Board &b,
	std::vector<int> &valid_moves
);

#endif 