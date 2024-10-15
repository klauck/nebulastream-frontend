//
// Created by Usama Bin Tariq on 23.09.24.
//

#include <memory>
#include <string>
#include <vector>
#include <nebula/parser/expressions/constant_expression.hpp>

namespace nebula
{
ConstantExpression::ConstantExpression(Value val)
    : ParsedExpression(ExpressionType::VALUE_CONSTANT, ExpressionClass::CONSTANT), value(std::move(val))
{
}

std::string ConstantExpression::ToString()
{
    return value == nullptr ? "" : value.toString();
}
}
