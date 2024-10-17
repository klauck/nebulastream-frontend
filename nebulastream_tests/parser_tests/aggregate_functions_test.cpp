#include <gtest/gtest.h>
#include <nebula/parser/expressions/column_expression.hpp>
#include <nebula/parser/expressions/function_expression.hpp>
#include <nebula/parser/nebula_parser.hpp>
#include <nebula/parser/refs/sub_query_ref.hpp>
#include <nebula/parser/statements/select_statement.hpp>

TEST(PARSER_TEST, AGGREGATE_FUNCTIONS_TEST)
{
    nebula::Parser parser;

    std::string query = "SELECT AVG(salary), MAX(hours) FROM employee_data";

    parser.parse(query);

    auto& statements = parser.statements;

    //assert the size of query
    ASSERT_EQ(1, statements.size());

    auto& first_statement = statements[0];

    //confirm the type of the query
    ASSERT_EQ(first_statement->type, nebula::StatementType::SELECT_STATEMENT);

    //cast statement into select statement
    auto& select_statement = first_statement->Cast<nebula::SelectStatement>();

    //extract node from select statement
    auto& node = select_statement.node;

    //make sure the node type is select node
    ASSERT_EQ(node->type, nebula::QueryNodeType::SELECT_NODE);

    auto& select_node = node->Cast<nebula::SelectNode>();

    //check the select size is 1
    ASSERT_EQ(select_node.select_list.size(), 2);

    auto& avg_select = select_node.select_list[0];
    auto& max_select = select_node.select_list[1];

    //check the avg select element is a function expression
    ASSERT_EQ(avg_select->type, nebula::ExpressionType::FUNCTION);

    //down cast parsed expression into function expression
    auto& avg_select_expr = avg_select->Cast<nebula::FunctionExpression>();

    //check function name
    ASSERT_EQ(avg_select_expr.function_name, "avg");

    //check the size of the children
    ASSERT_EQ(avg_select_expr.children.size(), 1);

    auto& avg_select_first_child = avg_select_expr.children[0];

    //check the type of the children
    ASSERT_EQ(avg_select_first_child->type, nebula::ExpressionType::COLUMN_REF);

    //down cast child into base table ref
    auto avg_table_ref = avg_select_first_child->Cast<nebula::ColumnExpression>();

    //check column size
    ASSERT_EQ(1, avg_table_ref.column_names.size());

    //check column name
    ASSERT_EQ(avg_table_ref.column_names[0], "salary");

    //check the max select element is a function expression
    ASSERT_EQ(max_select->type, nebula::ExpressionType::FUNCTION);

    //down cast parsed expression into function expression
    auto& max_select_expr = max_select->Cast<nebula::FunctionExpression>();

    //check function name
    ASSERT_EQ(max_select_expr.function_name, "max");

    //check the size of the children
    ASSERT_EQ(max_select_expr.children.size(), 1);

    auto& max_select_first_child = max_select_expr.children[0];

    //check the type of the children
    ASSERT_EQ(max_select_first_child->type, nebula::ExpressionType::COLUMN_REF);

    //down cast child into base table ref
    auto max_table_ref = max_select_first_child->Cast<nebula::ColumnExpression>();

    //check column size
    ASSERT_EQ(1, max_table_ref.column_names.size());

    //check column name
    ASSERT_EQ(max_table_ref.column_names[0], "hours");
}
