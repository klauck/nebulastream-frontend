// duckdb reference: src/parser/expression/columnref_expression.cpp
#include "nebula/parser/expressions/column_expression.hpp"
#include <memory>
#include <string>
#include <vector>
#include "nebula/parser/expressions/parsed_expression.hpp"

namespace nebula
{
ColumnExpression::ColumnExpression() : ParsedExpression(ExpressionType::COLUMN_REF, ExpressionClass::COLUMN_REF)
{
}

std::string ColumnExpression::ToString()
{
    std::string column_name;

    for (int i = 0; i < column_names.size(); i++)
    {
        if (i == 0)
            column_name = column_names[i];
        else
            column_name += "." + column_names[i];
    }

    if (!alias.empty())
    {
        column_name += " (" + alias + ")";
    }

    return column_name;
}
}
