#ifndef EVALUATION_H
#define EVALUATION_H

class Board;

// Once the total number of pieces on the board is at or below this value,
// the game has entered the endgame and we can change the eval function to
// alter the engine's behavior in the endgame
const int ENDGAME_THRESHOLD = 16;

int eval(Board &b, int game_over = 0);

#endif