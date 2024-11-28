//duckdb reference: src/include/duckdb/parser/statement/select_statement.hpp

#pragma once
#include <string>
#include "sql_statement.hpp"
#include <vector>
#include "nebula/parser/nodes/select_node.hpp"
#include <memory>

namespace nebula {
    class SelectStatement : public SQLStatement {
    public:
        SelectStatement();

        static constexpr const StatementType TYPE = StatementType::SELECT_STATEMENT;

        std::unique_ptr<QueryNode> node;

        void Print() const override;

        std::string ToString() const override;

        std::string ToStreamQuery() const override;

        bool VerifyStreamQuery() const override;
    };
}
