// duckdb reference: src/include/duckdb/parser/tableref/basetableref.hpp
#pragma once

#include <string>
#include "table_ref_type.hpp"

#include "table_ref.hpp"

namespace nebula
{
class BaseTableRef : public TableRef
{
public:
    static constexpr const TableReferenceType TYPE = TableReferenceType::BASE_TABLE;

    BaseTableRef() : TableRef(TableReferenceType::BASE_TABLE) { }

    std::string table_name;

    virtual ~BaseTableRef() = default;
};
}
