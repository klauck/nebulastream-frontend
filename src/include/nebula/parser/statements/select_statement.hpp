//
// Created by Usama Bin Tariq on 16.09.24.
//
#pragma once
#include <string>
#include "sql_statement.hpp"
#include <vector>

namespace nebula {
    class SelectStatement : public SQLStatement {
    public:
        static constexpr const StatementType TYPE = StatementType::SELECT_STATEMENT;

        std::unique_ptr<std::vector<std::string> > columns = std::make_unique<std::vector<std::string> >();
        std::unique_ptr<std::vector<std::string> > from = std::make_unique<std::vector<std::string> >();

        void Print() const override;

        std::string ToString() const override;

        std::string ToStreamQuery() const override;
    };
}
