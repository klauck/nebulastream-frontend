#include <nebula/parser/expressions/star_expression.hpp>

namespace nebula {
    StarExpression::StarExpression() : ParsedExpression(ExpressionType::STAR, ExpressionClass::STAR) {
    }

    std::string StarExpression::ToString() {
        return "*";
    }
}
