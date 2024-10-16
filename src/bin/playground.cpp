#include <string>
#include "nebula/parser/nebula_parser.hpp"
#include "nebula/parser/statements/select_statement.hpp"
#include "postgres_parser.hpp"


int main()
{
    const std::string query = "SELECT AVG(abc), MAX(b), SUM(c), COUNT(*) from a";
    std::cout << "Query: " << query << std::endl;

    nebula::Parser parser;
    const bool query_parsed = parser.parse(query);

    std::cout << "Query Parsed: " << query_parsed << std::endl;

    parser.PrintStatements();
}
