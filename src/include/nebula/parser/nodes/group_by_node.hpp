// duckdb reference: src/include/duckdb/parser/group_by_node.hpp
#pragma once

#include <memory>
#include <set>
#include <vector>
#include "nebula/parser/expressions/parsed_expression.hpp"

namespace nebula
{
class GroupByNode
{
public:
    std::vector<std::unique_ptr<ParsedExpression>> group_expressions;

    std::string ToString();
};
}
