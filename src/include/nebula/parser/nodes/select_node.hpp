// duckdb reference: src/include/duckdb/parser/select_node.hpp
#pragma once

#include <memory>
#include <vector>
#include "group_by_node.hpp"
#include "nebula/parser/expressions/parsed_expression.hpp"
#include "nebula/parser/refs/base_table_ref.hpp"
#include "order_by_node.hpp"
#include "query_node.hpp"


namespace nebula
{
class SelectNode : public QueryNode
{
public:
    SelectNode();

    static constexpr const QueryNodeType TYPE = QueryNodeType::SELECT_NODE;

    //! The projection list
    std::vector<std::unique_ptr<ParsedExpression>> select_list;
    //! The FROM clause
    std::vector<std::unique_ptr<TableRef>> from_tables;
    //! The WHERE clause
    std::unique_ptr<ParsedExpression> where_clause;
    //! The GROUP BY clause
    std::unique_ptr<GroupByNode> group_by;
    std::vector<std::unique_ptr<OrderByNode>> orders;

    std::string ToString() override;
};
}
