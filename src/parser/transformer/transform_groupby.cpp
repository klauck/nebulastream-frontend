// duckdb reference: src/parser/transform/expression/transform_groupby.cpp

#include <nebula/parser/transformer/transformer.hpp>


namespace nebula
{
std::unique_ptr<GroupByNode> Transformer::TransformGroupBy(pgquery::PGList* from)
{
    if (!from)
    {
        return nullptr;
    }

    std::unique_ptr<GroupByNode> group_by_node = std::make_unique<GroupByNode>();

    for (auto node = from->head; node != nullptr; node = node->next)
    {
        auto* cn = static_cast<pgquery::PGNode*>(node->data.ptr_value);

        auto expression = TransformExpression(cn);

        group_by_node->group_expressions.push_back(std::move(expression));
    }

    return group_by_node;
}
}
