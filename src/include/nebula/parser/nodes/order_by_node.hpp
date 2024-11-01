// duckdb reference: src/include/duckdb/parser/order_by_node.hpp
#pragma once

#include <memory>
#include <string>
#include "nebula/common/enums/order_by_type.hpp"
#include "nebula/parser/expressions/parsed_expression.hpp"

namespace nebula
{
class OrderByNode
{
public:
    OrderByNode(OrderType type, OrderByNullType null_order, std::unique_ptr<ParsedExpression> expression)
        : type(type), null_order(null_order), expression(std::move(expression))
    {
    }

    //! Sort order, ASC or DESC
    OrderType type;
    //! The NULL sort order, NULLS_FIRST or NULLS_LAST
    OrderByNullType null_order;
    //! Expression to order by
    std::unique_ptr<ParsedExpression> expression;

    std::string ToString();
};
}
