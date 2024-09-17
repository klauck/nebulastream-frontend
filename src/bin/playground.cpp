#include "postgres_parser.hpp"

#include <iostream>

#include "../parser/statements/includes/select_statement.h"

using namespace pg_parser;

int main() {

	PostgresParser parser;

	SelectStatement::Parse("");

	parser.Parse("SELECT * FROM t1;");
	std::cout << parser.success << std::endl;
}