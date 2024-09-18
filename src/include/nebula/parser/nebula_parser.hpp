//
// Created by Usama Bin Tariq on 18.09.24.
//

#pragma once
#include <iostream>
#include "statements/sql_statement.hpp"

namespace nebula {
    class Parser {
      public:
        Parser() = default;
        virtual ~Parser() = default;

        virtual void parse(const std::string& input) = 0;
        std::vector<std::unique_ptr<SQLStatement>> statements;
    };
}
