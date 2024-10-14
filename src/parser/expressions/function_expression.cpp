#include <utility>
#include "nebula/parser/expressions/function_expression.hpp"

namespace nebula {
    FunctionExpression::FunctionExpression() : ParsedExpression(ExpressionType::FUNCTION, ExpressionClass::FUNCTION) {
    }

    FunctionExpression::FunctionExpression(std::string function_name) : ParsedExpression(ExpressionType::FUNCTION,
                                                                            ExpressionClass::FUNCTION),
                                                                        function_name(std::move(function_name)) {
    }
}
