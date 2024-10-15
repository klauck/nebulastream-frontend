// duckdb reference: src/parser/expression/comparison_expression.cpp
#include <memory>
#include <string>
#include <vector>
#include <nebula/parser/expressions/comparison_expression.hpp>

namespace nebula
{
//constructor init, assign left and right operator
ComparisonExpression::ComparisonExpression(
    ExpressionType type, std::unique_ptr<ParsedExpression> left, std::unique_ptr<ParsedExpression> right)
    : ParsedExpression(type, ExpressionClass::COMPARISON), left(std::move(left)), right(std::move(right))
{
}

std::string ComparisonExpression::ToString()
{
    return "NOT IMPLEMENTED";
}
}
