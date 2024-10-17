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

    //all statement
    auto& statements = parser.statements;

    //print size of the statement
    printf("Parsed statements: %lu\n", statements.size());

    for(auto& stmt : statements)
    {
        printf("Statement Type: %s\n", StatementReturnTypeToString(stmt->type).c_str());

        //check if the type of the statement is select statement
        if(stmt->type == nebula::StatementType::SELECT_STATEMENT)
        {
            //down case the sql statement into select statement
            auto& select_statement = stmt->Cast<nebula::SelectStatement>();

            auto& query_node = select_statement.node;

            //check the type of query node
            if(query_node->type == nebula::QueryNodeType::SELECT_NODE)
            {
                //down cast query node into select node
                auto& select_node = query_node->Cast<nebula::SelectNode>();

                //get tables from select node
                auto& tables = select_node.from_tables;

                printf("Selected tables: %lu\n", tables.size());
                printf("Tables: ");

                for(auto& table : tables)
                {
                    //check type of the table
                    if(table->type == nebula::TableReferenceType::BASE_TABLE)
                    {
                        //down cast table into base table
                        auto& base_table = table->Cast<nebula::BaseTableRef>();

                        printf("%s ", base_table.table_name.c_str());
                    }
                }
                
                printf("\n");
            }
        }
    }

    return 0;
}
