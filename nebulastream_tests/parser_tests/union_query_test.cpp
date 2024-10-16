#include <gtest/gtest.h>
#include <nebula/parser/nebula_parser.hpp>
#include <string>
#include <nebula/parser/nodes/set_operation_node.hpp>
#include <nebula/parser/statements/select_statement.hpp>


TEST(PARSER_TEST, UNION_QUERY_TEST) {
    std::string query = "SELECT * FROM t1 UNION SELECT * FROM t2;";

    nebula::Parser parser;

    const auto parsed = parser.parse(query);

    //assert that the query is parsed correctly
    ASSERT_TRUE(parsed);

    //assert the parsed query size is 1
    ASSERT_EQ(parser.statements_collection->statements.size(), 1);

    // get first statement
    auto &statement = parser.statements_collection->statements[0];

    //assert that the statement is of type Select Statement
    ASSERT_EQ(statement->type, nebula::StatementType::SELECT_STATEMENT);
    // cast statement to SelectStatement
    auto select_statement = dynamic_cast<nebula::SelectStatement *>(statement.get());

    //assert that the select statement is not null
    ASSERT_TRUE(select_statement);

    //assert the query node is type of set_operation_node
    ASSERT_EQ(select_statement->node->type, nebula::QueryNodeType::SET_OPERATION_NODE);

    // cast the query node to SetOperationNode
    auto &son = select_statement->node->Cast<nebula::SetOperationNode>();
    // make sure left and right are not null
    ASSERT_TRUE(son.left);
    ASSERT_TRUE(son.right);

    // make sure left and right are of type SelectStatement
    ASSERT_EQ(son.left->type, nebula::QueryNodeType::SELECT_NODE);
    ASSERT_EQ(son.right->type, nebula::QueryNodeType::SELECT_NODE);

    // cast left and right to SelectStatement
    auto left = dynamic_cast<nebula::SelectNode *>(son.left.get());
    auto right = dynamic_cast<nebula::SelectNode *>(son.right.get());

    // make sure left and right are not null
    ASSERT_TRUE(left);
    ASSERT_TRUE(right);

    //very the columns and table nodes for left and right
    ASSERT_EQ(left->type, nebula::QueryNodeType::SELECT_NODE);
    ASSERT_EQ(right->type, nebula::QueryNodeType::SELECT_NODE);

    // cast left and right to SelectNode
    auto left_node = dynamic_cast<nebula::SelectNode *>(left);
    auto right_node = dynamic_cast<nebula::SelectNode *>(right);

    // make sure left and right are not null
    ASSERT_TRUE(left_node);
    ASSERT_TRUE(right_node);

    auto &from_table1 = left_node->from_tables[0];
    ASSERT_EQ(from_table1->type, nebula::TableReferenceType::BASE_TABLE);
    auto &from_table_ref1 = from_table1->Cast<nebula::BaseTableRef>();

    auto &from_table2 = right_node->from_tables[0];
    ASSERT_EQ(from_table2->type, nebula::TableReferenceType::BASE_TABLE);
    auto &from_table_ref2 = from_table2->Cast<nebula::BaseTableRef>();

    // verify the table names
    ASSERT_EQ(from_table_ref1.table_name, "t1");
    ASSERT_EQ(from_table_ref2.table_name, "t2");

    // verify the column names
    ASSERT_EQ(left_node->select_list.size(), 1);
    ASSERT_EQ(right_node->select_list.size(), 1);

    ASSERT_EQ(left_node->select_list[0]->type, nebula::ExpressionType::STAR);
    ASSERT_EQ(right_node->select_list[0]->type, nebula::ExpressionType::STAR);
}
