//duckdb reference: src/parser/transform/expression/transform_bool_expr.cpp

#include <nebula/parser/transformer/transformer.hpp>
#include <memory>
#include <nebula/common/exception.hpp>
#include <nebula/parser/expressions/conjunction_expression.hpp>

//method to convert the and / or node into parsed expression
std::unique_ptr<nebula::ParsedExpression> nebula::Transformer::TransformBool(pgquery::PGBoolExpr &expr) {
    std::unique_ptr<ParsedExpression> result;
    for (auto node = expr.args->head; node != nullptr; node = node->next) {
        auto cn = static_cast<pgquery::PGNode *>(node->data.ptr_value);
        auto next = TransformExpression(cn);

        switch (expr.boolop) {
            case pgquery::PG_AND_EXPR: {
                if (!result) {
                    result = std::move(next);
                } else {
                    result = std::make_unique<ConjunctionExpression>(ExpressionType::CONJUNCTION_AND, std::move(result),
                                                                     std::move(next));
                }
                break;
            }
            case pgquery::PG_OR_EXPR: {
                if (!result) {
                    result = std::move(next);
                } else {
                    result = std::make_unique<ConjunctionExpression>(ExpressionType::CONJUNCTION_OR, std::move(result),
                                                                     std::move(next));
                }
                break;
            }
            case pgquery::PG_NOT_EXPR: {
                if (next->type == ExpressionType::COMPARE_IN) {
                    // convert COMPARE_IN to COMPARE_NOT_IN, e.g not x in (1, 2, 3)
                    next->type = ExpressionType::COMPARE_NOT_IN;
                    result = std::move(next);
                } else if (next->type >= ExpressionType::COMPARE_EQUAL &&
                           next->type <= ExpressionType::COMPARE_GREATERTHANOREQUALTO) {
                    // NOT on a comparison: we can negate the comparison
                    // e.g. NOT(x > y) is equivalent to x <= y
                    next->type = NegateComparisonExpression(next->type);
                    result = std::move(next);
                } else {
                    //FIXME: Transform operator expression type
                }
                break;
            }
        }
    }

    return result;
}
