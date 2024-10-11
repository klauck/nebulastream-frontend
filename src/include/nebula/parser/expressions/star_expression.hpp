//duckdb reference: src/include/duckdb/parser/expression/star_expression.hpp

#pragma once
#include "parsed_expression.hpp"
#include <string>

namespace nebula {
    class StarExpression : public ParsedExpression {
    public:
        StarExpression();

        static constexpr const ExpressionClass TYPE = ExpressionClass::STAR;

        std::string ToString() override;
    };
}
