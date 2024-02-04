#include "trans_table.hpp"

int new_tt_entry(int node_type, int depth, int eval) {
	depth <<= 2;
	eval <<= 12;
	return node_type | depth | eval;
}
int get_node_type(int tt_entry) {
	return tt_entry & 3;
}
int get_depth(int tt_entry) {
	return (tt_entry >> 2) & 1023;
}
int get_eval(int tt_entry) {
	return (tt_entry >> 12) & 1048575;
}