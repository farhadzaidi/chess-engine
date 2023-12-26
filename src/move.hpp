#ifndef MOVE_H
#define MOVE_H

// Each move is represented by an integer
// Bits 1-6 indicate the origin square (0-63)
// Bits 7-12 indicate the destination square (0-63)
// Bits 13 indicates whether the move type (quiet or capture)
// Bits 14-16 indicate the move flag (special move)

int new_move(int from, int to, int mtype, int flag);
int get_from(int m);
int get_to(int m);
int get_mtype(int m);
int get_flag(int m);

#endif