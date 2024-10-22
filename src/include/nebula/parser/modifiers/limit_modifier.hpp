// duckdb reference: src/include/duckdb/parser/result_modifier.hpp
#pragma once

#include <string>
#include "result_modifier.hpp"

namespace nebula
{
class LimitModifier : public ResultModifier
{
public:
    static constexpr const ResultModifierType TYPE = ResultModifierType::LIMIT_MODIFIER;

    explicit LimitModifier() : ResultModifier(ResultModifierType::LIMIT_MODIFIER) { }

    explicit LimitModifier(int offset, int limit) : ResultModifier(ResultModifierType::LIMIT_MODIFIER), offset(offset), limit(limit) { }

    int offset = 0;
    int limit = 0;

    std::string ToString() override;
};
}
