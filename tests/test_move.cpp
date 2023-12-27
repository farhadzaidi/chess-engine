#include "catch.hpp"
#include "move.hpp"
#include "constants.hpp"

TEST_CASE("move_1") {
	int result_move = new_move(e2, e4, QUIET, NORMAL);
	int result_from = get_from(result_move);
	int result_to = get_to(result_move);
	int result_mtype = get_mtype(result_move);
	int result_flag = get_flag(result_move);

	int expected_move = 0b0000100100110100;
	int expected_from = 52;
	int expected_to = 36;
	int expected_mtype = 0;
	int expected_flag = 0;

	REQUIRE(result_move == expected_move);
	REQUIRE(result_from == expected_from);
	REQUIRE(result_to == expected_to);
	REQUIRE(result_mtype == expected_mtype);
	REQUIRE(result_flag == expected_flag);
}

TEST_CASE("move_2") {
	int result_move = new_move(c5, b6, CAPTURE, EN_PASSANT);
	int result_from = get_from(result_move);
	int result_to = get_to(result_move);
	int result_mtype = get_mtype(result_move);
	int result_flag = get_flag(result_move);

	int expected_move = 0b0011010001011010;
	int expected_from = 26;
	int expected_to = 17;
	int expected_mtype = 1;
	int expected_flag = 1;

	REQUIRE(result_move == expected_move);
	REQUIRE(result_from == expected_from);
	REQUIRE(result_to == expected_to);
	REQUIRE(result_mtype == expected_mtype);
	REQUIRE(result_flag == expected_flag);
}

TEST_CASE("move_3") {
	int result_move = new_move(e1, c1, QUIET, CASTLE);
	int result_from = get_from(result_move);
	int result_to = get_to(result_move);
	int result_mtype = get_mtype(result_move);
	int result_flag = get_flag(result_move);

	int expected_move = 0b0100111010111100;
	int expected_from = 60;
	int expected_to = 58;
	int expected_mtype = 0;
	int expected_flag = 2;

	REQUIRE(result_move == expected_move);
	REQUIRE(result_from == expected_from);
	REQUIRE(result_to == expected_to);
	REQUIRE(result_mtype == expected_mtype);
	REQUIRE(result_flag == expected_flag);
}

TEST_CASE("move_4") {
	int result_move = new_move(d7, c8, CAPTURE, P_QUEEN);
	int result_from = get_from(result_move);
	int result_to = get_to(result_move);
	int result_mtype = get_mtype(result_move);
	int result_flag = get_flag(result_move);
	
	int expected_move = 0b1101000010001011;
	int expected_from = 11;
	int expected_to = 2;
	int expected_mtype = 1;
	int expected_flag = 6;

	REQUIRE(result_move == expected_move);
	REQUIRE(result_from == expected_from);
	REQUIRE(result_to == expected_to);
	REQUIRE(result_mtype == expected_mtype);
	REQUIRE(result_flag == expected_flag);
}