#pragma once
#include <string>
#include "table_ref_type.hpp"

namespace nebula
{
class TableRef
{
public:
    static constexpr const TableReferenceType TYPE = TableReferenceType::INVALID;

    explicit TableRef(TableReferenceType type) : type(type) { }

    virtual ~TableRef() = default;

    template <class TARGET>
    inline TARGET& Cast()
    {
        if (type != TARGET::TYPE && TARGET::TYPE != TableReferenceType::INVALID)
        {
            throw InvalidOperationException("Failed to cast constraint to type - constraint type mismatch");
        }
        return reinterpret_cast<TARGET&>(*this);
    }

    TableReferenceType type;
    std::string alias;
};
}
