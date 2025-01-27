#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "statements/sql_statement_collection.hpp"

namespace nebula
{
class Parser
{
public:
    Parser();

    virtual ~Parser() = default;

    virtual bool parse(const std::string& input);

    std::unique_ptr<SQLStatementCollection> statements_collection = std::make_unique<SQLStatementCollection>();

    virtual void PrintStatements();
};
}
