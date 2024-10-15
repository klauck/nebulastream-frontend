// duckdb reference: src/include/duckdb/parser/expression/parsed_expression.hpp
#pragma once

#include <string>
#include "base_expression.hpp"

namespace nebula
{
class ParsedExpression : public BaseExpression
{
public:
    ParsedExpression(ExpressionType type, ExpressionClass expression_class) : BaseExpression(type, expression_class) { }

    virtual ~ParsedExpression() = default;

    std::string ToString() override;
};
}
