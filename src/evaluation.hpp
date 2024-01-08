#ifndef EVALUATION_H
#define EVALUATION_H

class Board;

const int PIECE_VALUE[7] = {0, 1, 3, 3, 5, 9, 100};

int eval(Board &b);


#endif