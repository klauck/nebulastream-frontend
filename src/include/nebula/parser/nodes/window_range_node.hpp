//
// Created by Usama Bin Tariq on 10.10.24.
//

#pragma once
#include <memory>
#include "nebula/parser/expressions/range_interval_expression.hpp"

namespace nebula {
    enum WindowRangeNodeType : short {
        SLIDING_WINDOW = 1,
        TUMBLING_WINDOW = 2,
    };

    class WindowRangeNode {
    public:
        explicit WindowRangeNode(WindowRangeNodeType type) : type(type) {
        }

        virtual ~WindowRangeNode() = default;

        WindowRangeNodeType type;
        std::unique_ptr<RangeIntervalExpression> range;
    };
}
