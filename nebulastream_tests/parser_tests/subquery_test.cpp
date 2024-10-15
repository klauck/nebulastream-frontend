#include <gtest/gtest.h>
#include <nebula/parser/nebula_parser.hpp>
#include <nebula/parser/refs/sub_query_ref.hpp>
#include <nebula/parser/statements/select_statement.hpp>

TEST(PARSER_TEST, SUB_QUERY_TEST) {
    nebula::Parser parser;

    std::string query = "select * from (select * from users)";

    parser.parse(query);

    auto &statements = parser.statements_collection->statements;

    //assert the size of query
    ASSERT_EQ(1, statements.size());

    auto &first_statement = statements[0];

    //confirm the type of the query
    ASSERT_EQ(first_statement->type, nebula::StatementType::SELECT_STATEMENT);

    //cast statement into select statement
    auto &select_statement = first_statement->Cast<nebula::SelectStatement>();

    //extract node from select statement
    auto &node = select_statement.node;

    //make sure the node type is select node
    ASSERT_EQ(node->type, nebula::QueryNodeType::SELECT_NODE);

    auto &select_node = node->Cast<nebula::SelectNode>();

    //check the select size is 1
    ASSERT_EQ(select_node.select_list.size(), 1);

    auto &first_select_item = select_node.select_list[0];

    //check the first select element is a star expression
    ASSERT_EQ(first_select_item->type, nebula::ExpressionType::STAR);

    //get from clause
    auto &from = select_node.from_table;

    //very that from is type of sub query
    ASSERT_EQ(from->type, nebula::TableReferenceType::SUBQUERY);

    //down cast to sub query ref
    auto &sub_query = from->Cast<nebula::SubQueryRef>();

    auto &sub_stmt = sub_query.subquery;

    //check type of subquery subquery
    ASSERT_EQ(sub_stmt->type, nebula::StatementType::SELECT_STATEMENT);

    //down cast select statement
    auto &sub_select = sub_stmt->Cast<nebula::SelectStatement>();

    //extract node from select statement
    auto &s_node = sub_select.node;

    //make sure the node type is select node
    ASSERT_EQ(s_node->type, nebula::QueryNodeType::SELECT_NODE);

    auto &s_select_node = s_node->Cast<nebula::SelectNode>();

    //check the select size is 1
    ASSERT_EQ(s_select_node.select_list.size(), 1);

    auto &s_first_select_item = select_node.select_list[0];

    //check the first select element is a star expression
    ASSERT_EQ(s_first_select_item->type, nebula::ExpressionType::STAR);

    //get from clause
    auto &s_from = s_select_node.from_table;

    ASSERT_EQ(s_from->type, nebula::TableReferenceType::BASE_TABLE);

    auto &s_from_table = s_from->Cast<nebula::BaseTableRef>();

    ASSERT_EQ("users", s_from_table.table_name);
}
