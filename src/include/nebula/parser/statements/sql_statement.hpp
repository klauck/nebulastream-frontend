#pragma once
#include <iostream>

#include "statement_type.hpp"

namespace nebula {
    class SQLStatement {
    public:
        static constexpr const StatementType TYPE = StatementType::INVALID_STATEMENT;

        virtual ~SQLStatement() = default;

        virtual std::string ToString() const = 0;

        virtual std::string ToStreamQuery() const = 0;

        virtual void Print() const = 0;
    };
}

