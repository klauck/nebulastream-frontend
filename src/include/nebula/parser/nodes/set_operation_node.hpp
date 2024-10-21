//duckdb reference: src/include/duckdb/parser/set_operation_node.hpp
#pragma once

#include <memory>
#include "query_node.hpp"

namespace nebula
{
enum class SetOperationType : short
{
    NONE = 0,
    UNION = 1,
    EXCEPT = 2,
    INTERSECT = 3,
    UNION_BY_NAME = 4
};

class SetOperationNode : public QueryNode
{
public:
    //the type of current node
    static constexpr const QueryNodeType TYPE = QueryNodeType::SET_OPERATION_NODE;

    SetOperationNode() : QueryNode(QueryNodeType::SET_OPERATION_NODE) { }

    //! The type of set operation
    SetOperationType setop_type = SetOperationType::NONE;
    //! whether the ALL modifier was used or not
    bool setop_all = false;
    //! The left side of the set operation
    std::unique_ptr<QueryNode> left;
    //! The right side of the set operation
    std::unique_ptr<QueryNode> right;

    std::string ToString() override;
};
}
