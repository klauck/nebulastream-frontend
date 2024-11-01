// duckdb reference: src/parser/transform/expression/transform_operator.cpp
#include <memory>
#include <string>
#include <vector>
#include <nebula/common/exception.hpp>
#include <nebula/parser/expressions/comparison_expression.hpp>
#include <nebula/parser/expressions/function_expression.hpp>
#include <nebula/parser/transformer/transformer.hpp>

namespace nebula
{
std::unique_ptr<ParsedExpression> Transformer::TransformAExpression(pgquery::PGAExpr& root)
{
    auto pg_val = static_cast<pgquery::PGValue*>(root.name->head->data.ptr_value);
    auto name = std::string(pg_val->val.str);
    //FIXME: Implement other types of expressions
    auto le = TransformExpression(root.lexpr);
    auto re = TransformExpression(root.rexpr);

    return TransformBinaryOperator(name, std::move(le), std::move(re));
}

std::unique_ptr<ParsedExpression>
Transformer::TransformBinaryOperator(std::string& op, std::unique_ptr<ParsedExpression> left, std::unique_ptr<ParsedExpression> right)
{
    auto target = OperatorToExpressionType(op);

    if (target != ExpressionType::INVALID)
    {
        return std::make_unique<ComparisonExpression>(target, std::move(left), std::move(right));
    }

    std::unique_ptr<FunctionExpression> expr_func = std::make_unique<FunctionExpression>(op);
    expr_func->children.emplace_back(std::move(left));
    expr_func->children.emplace_back(std::move(right));
    expr_func->is_operator = true;

    return std::move(expr_func);
}
}
