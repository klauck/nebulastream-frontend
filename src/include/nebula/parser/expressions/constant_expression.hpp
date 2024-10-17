// duckdb reference: src/include/duckdb/parser/expression/constant_expression.hpp
#pragma once

#include <string>
#include "nebula/common/value.hpp"
#include "parsed_expression.hpp"

namespace nebula
{
class ConstantExpression : public ParsedExpression
{
public:
    static constexpr const ExpressionClass TYPE = ExpressionClass::CONSTANT;

    explicit ConstantExpression(Value value);

    Value value;

    std::string ToString() override;
};
}
