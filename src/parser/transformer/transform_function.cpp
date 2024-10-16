// duckdb reference: src/parser/transform/expression/transform_function.cpp
#include <memory>
#include <string>
#include <nebula/parser/expressions/function_expression.hpp>
#include <nebula/parser/transformer/transformer.hpp>

namespace nebula
{
std::unique_ptr<ParsedExpression> Transformer::TransformFunction(pgquery::PGFuncCall& func)
{
    const auto& f_name = func.funcname;

    if (f_name->length != 1)
    {
        throw NotImplementedException("Catalog and/or Schema not implemented.");
    }

    const auto function_name = reinterpret_cast<pgquery::PGValue*>(f_name->head->data.ptr_value)->val.str;

    std::unique_ptr<FunctionExpression> function = std::make_unique<FunctionExpression>(function_name);

    function->children = std::move(Transformer::TransformExpressionList(func.args));

    return std::move(function);
}
}