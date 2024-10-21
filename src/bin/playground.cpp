#include <string>
#include "nebula/parser/nebula_parser.hpp"
#include "nebula/parser/statements/select_statement.hpp"
#include "postgres_parser.hpp"


int main()
{
    const std::string query = "SELECT first_name as fn, last_name as ln from users where id = 2 UNION SELECT first_name as fn, last_name "
                              "as ln from users where id = 2";
    std::cout << "Query: " << query << std::endl;

    nebula::Parser parser;
    const bool query_parsed = parser.parse(query);

    std::cout << "Query Parsed: " << query_parsed << std::endl;

    parser.PrintStatements();
}
