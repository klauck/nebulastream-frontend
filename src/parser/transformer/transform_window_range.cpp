
#include <nebula/parser/transformer/transformer.hpp>

namespace nebula {
    std::unique_ptr<WindowRangeNode> Transformer::TransformWindowRangeNode(pgquery::PGNode *node) {
        auto window_node = std::make_unique<WindowRangeNode>(WindowRangeNodeType::SLIDING_WINDOW);

        if (node->type == pgquery::T_PGWinRangeClause) {
            auto range_clause = reinterpret_cast<pgquery::PGWinRangeClause *>(node);

            auto interval = reinterpret_cast<pgquery::IntervalValue *>(range_clause->interval);

            window_node = std::make_unique<WindowRangeNode>(WindowRangeNodeType::TUMBLING_WINDOW);
            window_node->range = std::make_unique<RangeIntervalExpression>();
            window_node->range->unit = interval->unit;

            //convert value into pg const
            auto iv = reinterpret_cast<pgquery::PGAConst *>(interval->value);

            if (iv != nullptr) {
                window_node->range->value = iv->val.val.ival;
            }

            return std::move(window_node);
        }

        return nullptr;
    }
}
