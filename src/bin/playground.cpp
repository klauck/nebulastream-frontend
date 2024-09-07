#include "postgres_parser.hpp"
#include "duckdb/parser/transformer.hpp"
#include "duckdb/parser/parser_options.hpp"  

#include <iostream>

using namespace duckdb;

void ParseQuery(const std::string &query);

int main() {

	const string query = "SELECT * FROM table_name;";
	ParseQuery(query);
}

void ParseQuery(const std::string &query) {
	// Parser parser;
	// parser.ParseQuery(query);

	// parser.KeywordList();
// 	PostgresParser parser;
// 	parser.Parse(query);
// Parser::ParseQuery
// 	auto parse_tree = parser.parse_tree;

	ParserOptions options;
	Transformer transformer(options);

// 	vector<unique_ptr<SQLStatement>> statements;

// 	transformer.TransformParseTree(parse_tree, statements);

// 	// // print all statements
// 	// for (auto &statement : statements) {
// 	// 	statement->ToString();
// 	// }
}