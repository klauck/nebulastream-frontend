#include "postgres_parser.hpp"
#include "nebula/parser/nebula_parser.hpp"
#include "nebula/parser/statements/select_statement.hpp"
#include <string>
#include <parser/parser.hpp>


int main() {
	const std::string query = "SELECT abc FROM abc WINDOW RANGE 4 HOUR";

	std::cout << "Query: " << query << std::endl;

	nebula::Parser parser;
	const bool query_parsed = parser.parse(query);

	std::cout << "Query Parsed: " << query_parsed << std::endl;

	parser.PrintStatements();
}
