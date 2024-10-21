// duckdb reference: src/parser/expression/conjunction_expression.cpp
#include "nebula/parser/expressions/conjunction_expression.hpp"
#include <memory>
#include <string>
#include <vector>

namespace nebula
{
ConjunctionExpression::ConjunctionExpression(ExpressionType type) : ParsedExpression(type, ExpressionClass::CONJUNCTION)
{
}

ConjunctionExpression::ConjunctionExpression(ExpressionType type, std::vector<std::unique_ptr<ParsedExpression>> children)
    : ParsedExpression(type, ExpressionClass::CONJUNCTION)
{
    for (auto& child : children)
    {
        AddExpression(std::move(child));
    }
}

ConjunctionExpression::ConjunctionExpression(
    ExpressionType type, std::unique_ptr<ParsedExpression> left, std::unique_ptr<ParsedExpression> right)
    : ParsedExpression(type, ExpressionClass::CONJUNCTION)
{
    AddExpression(std::move(left));
    AddExpression(std::move(right));
}

void ConjunctionExpression::AddExpression(std::unique_ptr<ParsedExpression> expr)
{
    if (expr->type == type)
    {
        // expr is a conjunction of the same type: merge the expression lists together
        auto& other = expr->Cast<ConjunctionExpression>();
        for (auto& child : other.children)
        {
            children.push_back(std::move(child));
        }
    }
    else
    {
        children.push_back(std::move(expr));
    }
}

std::string ConjunctionExpression::ToString()
{
    std::string output;

    for (auto& expr : children)
    {
        output += expr->ToString();
    }

    return output;
}
}
