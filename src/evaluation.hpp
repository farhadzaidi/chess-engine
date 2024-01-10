#ifndef EVALUATION_H
#define EVALUATION_H

class Board;

const int PIECE_VALUE[7] = {0, 100, 300, 300, 500, 900, INF};

int eval(Board &b);


#endif