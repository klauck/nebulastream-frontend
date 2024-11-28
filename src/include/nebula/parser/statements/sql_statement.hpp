//duckdb reference: src/include/duckdb/parser/sql_statement.hpp

#pragma once
#include <iostream>
#include<string>
#include <nebula/common/exception.hpp>

#include "statement_type.hpp"


namespace nebula {
    //base class for all types of sql statements
    class SQLStatement {
    public:
        //default statement type is invalid statement as this is a base class
        static constexpr const StatementType TYPE = StatementType::INVALID_STATEMENT;

        explicit SQLStatement(StatementType type) : type(type) {
        }

        StatementType type;

        virtual ~SQLStatement() = default;

        //convert query to string
        virtual std::string ToString() const = 0;

        //get the string representation of the stream query
        virtual std::string ToStreamQuery() const = 0;

        //method to validate the query and check if there are unknown or restricted parameters or values.
        virtual bool VerifyStreamQuery() const = 0;

        //print the query to console
        virtual void Print() const = 0;

        template<class TARGET>
        TARGET &Cast() {
            if (type != TARGET::TYPE && TARGET::TYPE != StatementType::INVALID_STATEMENT) {
                throw InvalidOperationException("Failed to cast statement to type - statement type mismatch");
            }
            return reinterpret_cast<TARGET &>(*this);
        }
    };
}

