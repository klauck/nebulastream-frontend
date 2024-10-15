//
// Created by Usama Bin Tariq on 23.09.24.
//
#pragma once
#include <string>
#include "nebula/common/value.hpp"
#include "parsed_expression.hpp"

namespace nebula
{
class ConstantExpression : public ParsedExpression
{
public:
    explicit ConstantExpression(Value value);

    static constexpr const ExpressionClass TYPE = ExpressionClass::CONSTANT;
    Value value;

    std::string ToString() override;
};
}
