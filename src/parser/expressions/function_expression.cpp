// duckdb reference: src/parser/expression/function_expression.cpp
#include "nebula/parser/expressions/function_expression.hpp"
#include <string>
#include <utility>

namespace nebula
{
FunctionExpression::FunctionExpression() : ParsedExpression(ExpressionType::FUNCTION, ExpressionClass::FUNCTION)
{
}

FunctionExpression::FunctionExpression(std::string function_name)
    : ParsedExpression(ExpressionType::FUNCTION, ExpressionClass::FUNCTION), function_name(std::move(function_name))
{
}

std::string FunctionExpression::ToString()
{
    std::string output = function_name + "(";

    for (int i = 0; i < children.size(); ++i)
    {
        if (i == 0)
            output += children[i]->ToString();
        else
            output += "," + children[i]->ToString();
    }

    return output + ")";
}
}
