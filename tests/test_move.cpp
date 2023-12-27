#include "catch.hpp"
#include "move.hpp"
#include "constants.hpp"

TEST_CASE("Test1") {
	int expected_move = 0b0000100100110100;
	int expected_from = 52;
	int expected_to = 36;
	int expected_mtype = 0;
	int expected_flag = 0;

	int result_move = new_move(e2, e4, QUIET, NORMAL);
	int result_from = get_from(result_move);
	int result_to = get_to(result_move);
	int result_mtype = get_mtype(result_move);
	int result_flag = get_flag(result_move);

	REQUIRE(expected_move == result_move);
	REQUIRE(expected_from == result_from);
	REQUIRE(expected_to == result_to);
	REQUIRE(expected_mtype == result_mtype);
	REQUIRE(expected_flag == result_flag);
}