#ifndef DRAW_H
#define DRAW_H

#include <vector>

#include "SFML/Graphics.hpp"
#include "board.hpp"

const sf::Color LIGHT(245, 245, 245);
const sf::Color DARK(46, 46, 56);
const sf::Color INFO(127, 212, 245);

void draw_board(sf::RenderWindow &window, sf::Font &font);
void draw_pieces(sf::RenderWindow &window, Board &b);
void draw_moves(
	sf::RenderWindow &window, 
	Board &b, 
	int from, 
	std::vector<int> valid_moves
);



#endif 