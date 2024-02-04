#ifndef TRANS_TABLE_H
#define TRANS_TABLE_H

// Similar to moves, transposition table entries are encoded inside of integers
// Bits 1-2 indicate the type of node (Exact, Lower Bound, or Upper Bound)
// Bits 3-12 indicate the depth of the node
// Bits 13-32 indicate the evaluation of the node

int new_tt_entry(int node_type, int depth, int eval);
int get_node_type(int tt_entry);
int get_depth(int tt_entry);
int get_eval(int tt_entry);

#endif