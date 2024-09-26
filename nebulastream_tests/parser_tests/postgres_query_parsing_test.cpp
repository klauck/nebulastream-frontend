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

std::vector<std::string> read_file2(const std::string &filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return lines;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line[0] == '#') {
            // no sql query, but a comment line
            continue;
        }
        lines.push_back(line);
    }

    file.close();
    return lines;
}

TEST(POSTGRES_PARSER_TEST, QUERY_PARSING_TEST) {
//read queries from file
const auto queries = read_file2("./hyrise_queries.sql");

pg_parser::PostgresParser parser;
int total_parsed_queries = 0;

for (const auto &query: queries) {
  parser.Parse(query);
  ASSERT_TRUE(parser.parse_tree);
  total_parsed_queries++;
}

std::cout << "Total Parsed Queries: " << total_parsed_queries << std::endl;
}
