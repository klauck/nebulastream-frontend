// duckdb reference: src/include/duckdb/parser/expression/comparison_expression.hpp
#pragma once

#include <memory>
#include <string>
#include "parsed_expression.hpp"

namespace nebula
{
class ComparisonExpression : public ParsedExpression
{
public:
    static constexpr const ExpressionClass TYPE = ExpressionClass::COMPARISON;

    explicit ComparisonExpression(ExpressionType type, std::unique_ptr<ParsedExpression> left, std::unique_ptr<ParsedExpression> right);

    std::unique_ptr<ParsedExpression> left;
    std::unique_ptr<ParsedExpression> right;

    std::string ToString() override;
};
}
