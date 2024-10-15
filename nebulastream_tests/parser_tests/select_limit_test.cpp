#include <gtest/gtest.h>
#include <nebula/parser/nebula_parser.hpp>
#include <nebula/parser/modifiers/limit_modifier.hpp>
#include <nebula/parser/statements/select_statement.hpp>
TEST(PARSER_TEST, SELECT_LIMIT_TEST) {
    const std::string query = "SELECT * FROM test limit 10 offset 10";

    nebula::Parser parser;

    parser.parse(query);

    const auto &statements = parser.statements_collection->statements;

    ASSERT_EQ(statements.size(), 1);

    auto select_statement = dynamic_cast<nebula::SelectStatement *>(statements[0].get());

    //make sure the results modifiers length is 1
    ASSERT_EQ(select_statement->node->modifiers.size(), 1);

    auto &first_modifier = select_statement->node->modifiers[0];

    //assert the type of the modifier
    ASSERT_EQ(first_modifier->type, nebula::ResultModifierType::LIMIT_MODIFIER);

    auto limit_modifier = first_modifier->Cast<nebula::LimitModifier>();

    //assert the limit and offset
    ASSERT_EQ(limit_modifier.limit, 10);
    ASSERT_EQ(limit_modifier.offset, 10);
}
