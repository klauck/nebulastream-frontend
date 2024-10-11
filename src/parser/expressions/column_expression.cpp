//duckdb reference: src/parser/expression/columnref_expression.cpp

#include "nebula/parser/expressions/column_expression.hpp"
#include "nebula/parser/expressions/parsed_expression.hpp"
#include <string>
#include <vector>
#include <memory>


namespace nebula {
    ColumnExpression::ColumnExpression() : ParsedExpression(ExpressionType::COLUMN_REF, ExpressionClass::COLUMN_REF) {
    }

    std::string ColumnExpression::ToString() {
        return "NOT IMPLEMENTED";
    }
}
