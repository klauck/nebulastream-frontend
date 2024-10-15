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
    return "NOT IMPLEMENTED";
}
}
