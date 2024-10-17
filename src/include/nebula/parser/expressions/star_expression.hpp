// duckdb reference: src/include/duckdb/parser/expression/star_expression.hpp
#pragma once

#include <string>
#include "parsed_expression.hpp"

namespace nebula
{
class StarExpression : public ParsedExpression
{
public:
    static constexpr const ExpressionClass TYPE = ExpressionClass::STAR;

    StarExpression();

    std::string ToString() override;
};
}
