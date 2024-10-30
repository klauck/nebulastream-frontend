#include <string>
#include "nebula/parser/nebula_parser.hpp"
#include "nebula/parser/statements/select_statement.hpp"
#include "postgres_parser.hpp"


int main()
{
    const std::string query = R"(SELECT l_orderkey, SUM(l_extendedprice) as revenue, o_orderdate, o_shippriority
      FROM customer, orders, lineitem
      WHERE c_mktsegment = 2 AND c_custkey = o_custkey AND l_orderkey = o_orderkey
      AND o_orderdate < 2 AND l_shipdate > 2
      GROUP BY l_orderkey, o_orderdate, o_shippriority
      ORDER BY revenue DESC, o_orderdate
      LIMIT 10;)";

    std::cout << "Query: " << query << std::endl;

    nebula::Parser parser;
    const bool query_parsed = parser.parse(query);

    std::cout << "Query Parsed: " << query_parsed << std::endl;

    parser.PrintStatements();
}
