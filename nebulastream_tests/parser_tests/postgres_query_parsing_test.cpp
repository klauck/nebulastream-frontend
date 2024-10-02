//
// Created by Stefan Halfpap on 25.09.24.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "postgres_parser.hpp"
#include <unistd.h>
#include "helpers/helper_functions.hpp"

TEST(POSTGRES_PARSER_TEST, QUERY_PARSING_TEST) {
    //read queries from file
    const auto queries = nebula_tests::read_file("./hyrise_queries.sql");
    int total_parsed_queries = 0;

    for (const auto &query: queries) {
        pg_parser::PostgresParser parser;
        parser.Parse(query);

        if (parser.success)
            total_parsed_queries++;
    }

    ASSERT_EQ(total_parsed_queries, queries.size());

    std::cout << "Total Parsed Queries: " << total_parsed_queries << std::endl;
}
