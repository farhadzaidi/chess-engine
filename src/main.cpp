#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
	// Run unit tests
    Catch::Session().run(argc, argv);
}