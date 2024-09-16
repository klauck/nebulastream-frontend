#include "postgres_parser.hpp"

#include <iostream>

using namespace pg_parser;

int main() {

	PostgresParser parser;

	parser.Parse("SELECT * FROM t1;");
	std::cout << parser.success << std::endl;
}