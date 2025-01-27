// duckdb reference: src/parser/transform/expression/transform_columnref.cpp
#include <memory>
#include <string>
#include <vector>
#include <nebula/common/exception.hpp>
#include <nebula/parser/expressions/column_expression.hpp>
#include <nebula/parser/expressions/parsed_expression.hpp>
#include <nebula/parser/transformer/transformer.hpp>
#include <parser/gramparse.hpp>

namespace nebula
{
std::unique_ptr<ParsedExpression> Transformer::TransformColumnRef(pgquery::PGColumnRef& ref)
{
    auto result = std::make_unique<ColumnExpression>();
    auto head_node = static_cast<pgquery::PGNode*>(ref.fields->head->data.ptr_value);

    if (head_node->type == pgquery::T_PGString)
    {
        for (auto node = ref.fields->head; node; node = node->next)
        {
            const auto value = static_cast<pgquery::PGValue*>(node->data.ptr_value);
            result->column_names.push_back(value->val.str);
        }

        return result;
    }

    throw NotImplementedException("Parsing column ref failed");
}
}
