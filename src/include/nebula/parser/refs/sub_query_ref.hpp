// duckdb reference: src/include/duckdb/parser/tableref/subqueryref.hpp
#pragma once

#include <memory>
#include "nebula/parser/statements/select_statement.hpp"
#include "table_ref.hpp"

namespace nebula
{
class SubQueryRef : public TableRef
{
private:
    SubQueryRef() : TableRef(TableReferenceType::SUBQUERY) { }

public:
    static constexpr const TableReferenceType TYPE = TableReferenceType::SUBQUERY;

    explicit SubQueryRef(std::unique_ptr<SelectStatement> subquery) : TableRef(TableReferenceType::SUBQUERY), subquery(std::move(subquery))
    {
    }

    std::unique_ptr<SelectStatement> subquery;
};
}
