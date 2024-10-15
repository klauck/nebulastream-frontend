//
// Created by Usama Bin Tariq on 07.10.24.
//
#pragma once
#include "table_ref.hpp"
#include "nebula/parser/statements/select_statement.hpp"
#include <memory>

namespace nebula {
    class SubQueryRef : public TableRef {
    private:
        SubQueryRef() : TableRef(TableReferenceType::SUBQUERY) {
        }

    public:
        static constexpr const TableReferenceType TYPE = TableReferenceType::SUBQUERY;

        explicit SubQueryRef(std::unique_ptr<SelectStatement> subquery) : TableRef(TableReferenceType::SUBQUERY),
                                                                          subquery(std::move(subquery)) {
        }

        std::unique_ptr<SelectStatement> subquery;
    };
}
