#pragma once

#include "result_modifier.hpp"

namespace nebula {
    class LimitModifier : public ResultModifier {
    public:
        static constexpr const ResultModifierType TYPE = ResultModifierType::LIMIT_MODIFIER;

        explicit LimitModifier() : ResultModifier(ResultModifierType::LIMIT_MODIFIER) {
        }

        explicit LimitModifier(int offset, int limit) : ResultModifier(ResultModifierType::LIMIT_MODIFIER),
                                                        offset(offset), limit(limit) {
        }

        int offset = 0;
        int limit = 0;
    };
}
