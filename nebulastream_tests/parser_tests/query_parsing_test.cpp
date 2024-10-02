//
// Created by Usama Bin Tariq on 25.09.24.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include <nebula/parser/nebula_parser.hpp>
#include <unistd.h>
#include "helpers/helper_functions.hpp"

TEST(PARSER_TEST, QUERY_PARSING_TEST) {
    //read queries from file
    const auto queries = nebula_tests::read_file("./queries.sql");

    nebula::Parser parser;
    int total_parsed_queries = 0;

    for (const auto &query: queries) {
        parser.parse(query);

        ASSERT_EQ(++total_parsed_queries, parser.statements_collection->statements.size());
    }

    std::cout << "Total Parsed Queries: " << total_parsed_queries << std::endl;
}
