#include <postgres_parser.hpp>


#include "iostream"
#include "tpch_queries.hpp"

int main()
{
    auto &queries = nebula_benchmark::TpchQueries::queries;
    std::vector<std::string> failed_queries ;
    std::vector<std::string> passed_queries ;

    for(auto & cq : queries)
    {
        pg_parser::PostgresParser parser;

        parser.Parse(cq);

        if(parser.parse_tree == nullptr)
        {
            failed_queries.push_back(cq);
        }else
        {
            passed_queries.push_back(cq);
        }
    }

    std::cout << "Failed Queries: " << failed_queries.size() << std::endl;
    std::cout << "Passed Queries: " << passed_queries.size() << std::endl;

    return 0;
}