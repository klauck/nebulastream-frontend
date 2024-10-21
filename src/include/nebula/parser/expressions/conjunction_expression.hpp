// duckdb reference: src/include/duckdb/parser/expression/conjunction_expression.hpp
#pragma once

#include <memory>
#include <string>
#include <vector>
#include "parsed_expression.hpp"

//this class represents 'and' and 'or' expressions
namespace nebula
{
class ConjunctionExpression : public ParsedExpression
{
public:
    static constexpr const ExpressionClass TYPE = ExpressionClass::CONJUNCTION;

    explicit ConjunctionExpression(ExpressionType type);

    ConjunctionExpression(ExpressionType type, std::vector<std::unique_ptr<ParsedExpression>> children);

    ConjunctionExpression(ExpressionType type, std::unique_ptr<ParsedExpression> left, std::unique_ptr<ParsedExpression> right);

    std::vector<std::unique_ptr<ParsedExpression>> children;

    void AddExpression(std::unique_ptr<ParsedExpression> expr);

    std::string ToString() override;
};
}
