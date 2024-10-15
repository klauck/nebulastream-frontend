//
// Created by Usama Bin Tariq on 18.09.24.
//

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <nebula/parser/statements/sql_statement.hpp>

namespace nebula {
    class Parser {
    public:
        Parser();

        virtual ~Parser() = default;

        virtual bool parse(const std::string &input);

        std::vector<std::unique_ptr<SQLStatement> > statements;

        virtual void PrintStatements();
    };
}
