//
// Created by Usama Bin Tariq on 01.10.24.
//


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
            default: throw NotImplementedException("This type of expression is not implemented.");

            //FIXME: Add NOT Case
        }
    }

    return result;
}
