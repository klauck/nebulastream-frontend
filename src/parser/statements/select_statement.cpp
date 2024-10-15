//duckdb reference: src/parser/statement/select_statement.cpp

#include <iostream>
#include "nebula/parser/statements/select_statement.hpp"
#include <string>
#include <vector>
#include <memory>

namespace nebula {
    constexpr const StatementType nebula::SelectStatement::TYPE;

    SelectStatement::SelectStatement() : SQLStatement(StatementType::SELECT_STATEMENT) {
    }

    void SelectStatement::Print() const {
        std::cout << "=====Select Statement=====" << std::endl;
    }

    std::string SelectStatement::ToString() const {
        return std::string("Select");
    }

    bool SelectStatement::VerifyStreamQuery() const {
        return true;
    }

    std::string SelectStatement::ToStreamQuery() const {
        // if (from->empty()) {
        //     return "No From Clause";
        // }
        // //from by , seprated
        // std::string from_joined = "";
        // int count = 0;
        // for (const auto &fr: *from) {
        //     from_joined += fr + (from->size() == (++count) ? "" : ",");
        // }
        //
        // return "Query::from(\"" + from_joined + "\")";
        return "";
    }
}
