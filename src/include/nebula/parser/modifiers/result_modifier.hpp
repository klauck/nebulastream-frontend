// duckdb reference: src/include/duckdb/parser/result_modifier.hpp
#pragma once
#include <string>

namespace nebula
{
enum class ResultModifierType : short
{
    LIMIT_MODIFIER = 1,
    ORDER_MODIFIER = 2,
    DISTINCT_MODIFIER = 3,
    LIMIT_PERCENT_MODIFIER = 4
};

class ResultModifier
{
public:
    explicit ResultModifier(ResultModifierType type) : type(type) { }

    ResultModifierType type;

    virtual std::string ToString() = 0;

    template <class TARGET>
    TARGET& Cast()
    {
        if (type != TARGET::TYPE)
        {
            throw std::runtime_error("Failed to cast expression to type - expression type mismatch");
        }
        return reinterpret_cast<TARGET&>(*this);
    }
};
}
