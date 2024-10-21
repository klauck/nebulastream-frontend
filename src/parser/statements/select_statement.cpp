// duckdb reference: src/parser/statement/select_statement.cpp
#include "nebula/parser/statements/select_statement.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace nebula
{
constexpr const StatementType nebula::SelectStatement::TYPE;

SelectStatement::SelectStatement() : SQLStatement(StatementType::SELECT_STATEMENT)
{
}

void SelectStatement::Print()
{
    std::cout << "=====Select Statement=====" << std::endl;
    std::cout << ToString() << std::endl;
    std::cout << "=====END STATEMENT======" << std::endl;
}

std::string SelectStatement::ToString()
{
    return node->ToString();
}

bool SelectStatement::VerifyStreamQuery() const
{
    return true;
}

std::string SelectStatement::ToStreamQuery() const
{
    return "";
}
}
