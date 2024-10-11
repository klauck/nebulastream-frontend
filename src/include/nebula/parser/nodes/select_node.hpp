//duckdb reference: src/include/duckdb/parser/select_node.hpp

#pragma once
#include "nebula/parser/expressions/parsed_expression.hpp"
#include "nebula/parser/refs/base_table_ref.hpp"
#include "query_node.hpp"
#include <vector>
#include <memory>
#include "window_range_node.hpp"


namespace nebula {
    class SelectNode : public QueryNode {
    public:
        SelectNode();

        static constexpr const QueryNodeType TYPE = QueryNodeType::SELECT_NODE;

        //! The projection list
        std::vector<std::unique_ptr<ParsedExpression> > select_list;
        //! The FROM clause
        std::vector<std::unique_ptr<TableRef> > from_tables;
        //! The WHERE clause
        std::unique_ptr<ParsedExpression> where_clause;
        std::unique_ptr<WindowRangeNode> window_clause;
    };
}
