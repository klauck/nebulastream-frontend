//duckdb reference: src/parser/expression/constant_expression.cpp

#include <nebula/parser/expressions/constant_expression.hpp>
#include <string>
#include <vector>
#include <memory>

namespace nebula {
    ConstantExpression::ConstantExpression(Value val) : ParsedExpression(ExpressionType::VALUE_CONSTANT,
                                                                         ExpressionClass::CONSTANT),
                                                        value(std::move(val)) {
    }

    std::string ConstantExpression::ToString() {
        return value == nullptr ? "" : value.toString();
    }
}
