// duckdb reference: src/parser/transform/expression/transform_orderby.cpp

#include <nebula/parser/transformer/transformer.hpp>

namespace nebula
{
std::vector<std::unique_ptr<OrderByNode>> Transformer::TransformOrderBy(pgquery::PGList* order)
{
    std::vector<std::unique_ptr<OrderByNode>> orderby_nodes;

    if (order == nullptr)
    {
        return orderby_nodes;
    }

    for (auto node = order->head; node != nullptr; node = node->next)
    {
        auto* temp_node = static_cast<pgquery::PGNode*>(node->data.ptr_value);

        if (temp_node->type != pgquery::T_PGSortBy)
        {
            throw NotImplementedException("ORDER BY list member");
        }

        OrderType type;
        OrderByNullType null_order;
        auto sort = PGCast<pgquery::PGSortBy>(*temp_node);
        auto target = sort.node;

        if (sort.sortby_dir == pgquery::PG_SORTBY_DEFAULT)
        {
            type = OrderType::ORDER_DEFAULT;
        }
        else if (sort.sortby_dir == pgquery::PG_SORTBY_ASC)
        {
            type = OrderType::ASCENDING;
        }
        else if (sort.sortby_dir == pgquery::PG_SORTBY_DESC)
        {
            type = OrderType::DESCENDING;
        }
        else
        {
            throw NotImplementedException("Unimplemented order by type");
        }

        if (sort.sortby_nulls == pgquery::PG_SORTBY_NULLS_DEFAULT)
        {
            null_order = OrderByNullType::ORDER_DEFAULT;
        }
        else if (sort.sortby_nulls == pgquery::PG_SORTBY_NULLS_FIRST)
        {
            null_order = OrderByNullType::NULLS_FIRST;
        }
        else if (sort.sortby_nulls == pgquery::PG_SORTBY_NULLS_LAST)
        {
            null_order = OrderByNullType::NULLS_LAST;
        }
        else
        {
            throw NotImplementedException("Unimplemented order by type");
        }

        auto order_expression = TransformExpression(target);
        std::unique_ptr<OrderByNode> orderby_node = std::make_unique<OrderByNode>(type, null_order, std::move(order_expression));

        orderby_nodes.emplace_back(std::move(orderby_node));
    }

    return std::move(orderby_nodes);
}
}
