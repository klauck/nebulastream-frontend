#pragma once

namespace nebula {
    enum class ResultModifierType : short {
        LIMIT_MODIFIER = 1,
        ORDER_MODIFIER = 2,
        DISTINCT_MODIFIER = 3,
        LIMIT_PERCENT_MODIFIER = 4
    };
    class ResultModifier {
        public:
        explicit ResultModifier(ResultModifierType type) : type(type) {}

        ResultModifierType type;
    };
}