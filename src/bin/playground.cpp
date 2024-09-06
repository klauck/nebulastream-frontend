#include "postgres_parser.hpp"
#include "duckdb/parser/transformer.hpp"

#include <iostream>

int main() {

	duckdb::PostgresParser parser;

	parser.Parse("SELECT * FROM t1;");

	auto tree_type = parser.parse_tree;

	// Parser parser;

	std::cout << parser.success << std::endl;
}