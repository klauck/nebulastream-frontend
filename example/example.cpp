#include <cstdio>
#include <string>
#include <nebula/parser/nebula_parser.hpp>
#include <nebula/parser/statements/select_statement.hpp>

int main(const int ac, const char **av)
{
    if(ac != 2)
    {
        printf("Usage: ./example <sql_query>\n");
        return 0;
    }

    const std::string query = av[1];

    //create parser instance
    nebula::Parser parser;

    //parse query
    auto is_query_parsed = parser.parse(query);

    //check if the query has been parsed or not
    if(!is_query_parsed)
    {
        printf("Failed to parse query string: %s\n", query.c_str());
        return 0;
    }

    printf("Parsed query string: %s\n", query.c_str());

    //print the parsed queries
    parser.PrintStatements();

    return 0;
}
