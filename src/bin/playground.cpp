#include "postgres_parser.hpp"

#include <iostream>

int main() {

	duckdb::PostgresParser parser;

	parser.Parse("SELECT * FROM t1;");
	std::cout << parser.success << std::endl;
}