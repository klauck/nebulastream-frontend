//duckdb reference: src/include/duckdb/parser/expression/conjunction_expression.hpp

#pragma once
#include "parsed_expression.hpp"
#include <vector>
#include <string>
#include <memory>

//this class represents 'and' and 'or' expressions
namespace nebula {
    class ConjunctionExpression : public ParsedExpression {
    public:
        static constexpr const ExpressionClass TYPE = ExpressionClass::CONJUNCTION;

        explicit ConjunctionExpression(ExpressionType type);

        ConjunctionExpression(ExpressionType type, std::vector<std::unique_ptr<ParsedExpression> > children);

        ConjunctionExpression(ExpressionType type, std::unique_ptr<ParsedExpression> left,
                              std::unique_ptr<ParsedExpression> right);

        std::vector<std::unique_ptr<ParsedExpression> > children;

        void AddExpression(std::unique_ptr<ParsedExpression> expr);

        std::string ToString() const;
    };
}
