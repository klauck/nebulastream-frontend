//
// Created by Usama Bin Tariq on 10.10.24.
//

#pragma once
#include "parsed_expression.hpp"
#include "base_expression.hpp"
#include "string"

namespace nebula {
    class RangeIntervalExpression : public ParsedExpression {
      public:
        RangeIntervalExpression() : ParsedExpression(ExpressionType::RANGE_INTERVAL, ExpressionClass::WINDOW_RANGE){}
        int value;
        std::string unit;
    };
}