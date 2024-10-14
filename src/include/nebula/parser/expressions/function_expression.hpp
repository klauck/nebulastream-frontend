//duckdb reference: src/include/duckdb/parser/expression/function_expression.hpp

#include "parsed_expression.hpp"
#include <vector>
#include <memory>
#include <string>

namespace nebula {

    class FunctionExpression : public ParsedExpression {
        static constexpr const ExpressionClass TYPE = ExpressionClass::FUNCTION;
        public:
          FunctionExpression();
          explicit FunctionExpression(std::string function_name);

          std::string function_name;
          std::vector<std::unique_ptr<ParsedExpression>> children;
    };

}