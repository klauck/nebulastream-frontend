// duckdb reference: src/include/duckdb/parser/expression/base_expression.hpp
#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include "expression_types.hpp"

namespace nebula
{
class BaseExpression
{
public:
    BaseExpression(ExpressionType type, ExpressionClass expression_class) : type(type), expression_class(expression_class) { }

    template <class TARGET>
    TARGET& Cast()
    {
        if (expression_class != TARGET::TYPE)
        {
            throw std::runtime_error("Failed to cast expression to type - expression type mismatch");
        }
        return reinterpret_cast<TARGET&>(*this);
    }

    virtual ~BaseExpression() = default;

    virtual std::string ToString() = 0;

    std::string alias;

    ExpressionType type;
    ExpressionClass expression_class;
};
}
