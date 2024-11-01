//duckdb reference: src/include/duckdb/parser/expression/function_expression.hpp
#include <memory>
#include <string>
#include <vector>
#include "parsed_expression.hpp"

namespace nebula
{
class FunctionExpression : public ParsedExpression
{
public:
    static constexpr const ExpressionClass TYPE = ExpressionClass::FUNCTION;

    FunctionExpression();
    explicit FunctionExpression(std::string function_name);

    std::string function_name;
    std::vector<std::unique_ptr<ParsedExpression>> children;
    bool is_operator;

    std::string ToString() override;
};
}
