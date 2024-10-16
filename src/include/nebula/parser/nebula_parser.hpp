#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "statements/sql_statement.hpp"

namespace nebula
{
class Parser
{
public:
    Parser();

    virtual ~Parser() = default;

    virtual bool parse(const std::string& input);

    std::vector<std::unique_ptr<SQLStatement>> statements;

    virtual void PrintStatements();
};
}
