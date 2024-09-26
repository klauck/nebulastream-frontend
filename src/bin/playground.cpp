#include "postgres_parser.hpp"
#include "nebula/parser/nebula_parser.hpp"
#include "nebula/parser/statements/select_statement.hpp"
#include <string>


int main() {
	const std::string query = "SELECT a as b FROM test where x = 2";
	std::cout << "Query: " << query << std::endl;

	nebula::Parser parser;
	const bool query_parsed = parser.parse(query);

	std::cout << "Query Parsed: " << query_parsed << std::endl;

	parser.PrintStatements();

	std::cout << "=====Stream Queries=====" << std::endl << std::endl;
	for (const auto &stmt: parser.statements_collection->statements) {
		std::cout << stmt->ToStreamQuery() << std::endl;
	}

    pg_parser::PostgresParser parser2;
    parser2.Parse("SELECT a as b FROM test where x = 2");
}
