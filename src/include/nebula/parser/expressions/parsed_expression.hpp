#pragma once

#include "base_expression.hpp"

namespace nebula {
    class ParsedExpression : public BaseExpression {
    public:
        ParsedExpression() = default;

        virtual ~ParsedExpression() = default;

        std::string ToString() override;
    };
}
