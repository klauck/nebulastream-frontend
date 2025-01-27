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
    explicit ComparisonExpression(ExpressionType type, std::unique_ptr<ParsedExpression> left, std::unique_ptr<ParsedExpression> right);

    static constexpr const ExpressionClass TYPE = ExpressionClass::COMPARISON;
    std::unique_ptr<ParsedExpression> left;
    std::unique_ptr<ParsedExpression> right;

    std::string ToString() override;
};
}
