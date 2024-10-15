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
    explicit ConstantExpression(Value value);

    static constexpr const ExpressionClass TYPE = ExpressionClass::CONSTANT;
    Value value;

    std::string ToString() override;
};
}
