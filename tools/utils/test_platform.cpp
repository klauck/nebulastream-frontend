#include "duckdb/common/platform.hpp"

#include <duckdb/parser/parser.hpp>
#include <iostream>


using namespace std;

int main() {
	const std::string query = "select * from duckdb";

	duckdb::Parser parser;

	parser.ParseQuery(query);

	cout << "size: " << parser.statements.size() << endl;
	const auto& statements = parser.statements; // Use a const reference to avoid copying

	for (const auto &stmt : statements) {
		auto &sql_statement = *stmt;
		cout << StatementTypeToString(stmt->type) << '\n';
	}

	return 0;
}
