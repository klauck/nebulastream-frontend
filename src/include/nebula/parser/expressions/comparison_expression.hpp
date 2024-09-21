#include "parsed_expression.hpp"
#include "expression_types.hpp"
#include<memory>

namespace nebula {
    class ComparisonExpression : public ParsedExpression {
        public: 
            static constexpr const ExpressionClass TYPE = ExpressionClass::COMPARISON;
            std::unique_ptr<ParsedExpression> left;
            std::unique_ptr<ParsedExpression> right;
    };
}