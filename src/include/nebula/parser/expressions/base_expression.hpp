#pragma once

#include<iostream>
#include "expression_types.hpp"
#include <string>

namespace nebula {
    class BaseExpression {
    public:
        BaseExpression() = default;

        virtual ~BaseExpression() = default;

        virtual std::string ToString() = 0;

        std::unique_ptr<std::string> alias;

        ExpressionType type;
        ExpressionClass expression_class;
    };
}
