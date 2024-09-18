//
// Created by Usama Bin Tariq on 18.09.24.
//

#pragma once
#include <iostream>
#include "statements/sql_statement_collection.hpp"

namespace nebula {
    class Parser {
    public:
        Parser();

        virtual ~Parser() = default;

        virtual bool parse(const std::string &input);

        std::unique_ptr<SQLStatementCollection> statements_collection = std::make_unique<
            SQLStatementCollection>();

        virtual void PrintStatements();
    };
}
