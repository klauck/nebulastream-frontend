#include <iostream>
#include <duckdb/parser/parser.hpp>
#include <duckdb/parser/query_node/select_node.hpp>

using namespace std;
using namespace duckdb;

int main()
{
    const string query = "SELECT * FROM nebula_data;update nebula_data set a = 1 where b = 2;";

    cout << "Query: " << query << endl;

    Parser parser;

    try{
        parser.ParseQuery(query);
    } catch (std::exception &e) {
        cout << "Exception: " << e.what() << endl;
        return -1;
    }

    cout << "Query Parsed" << endl;
    cout << "Query size: " << parser.statements.size() << endl;
	const auto& statements = parser.statements; // Use a const reference to avoid copying

	for (const auto &stmt : statements) {
        auto& statement = *stmt;
        

        if(statement.type == StatementType::SELECT_STATEMENT){
            auto &st = (*stmt).Cast<SelectStatement>();


            //check if st node type is equal to selectnode type

            auto &node = *st.node;

            if(*node.type == SelectNode::type){}

            cout << "Select Statement Parsed" << '\n';
        } else {
            cout << "Unknown Statement" << '\n';
        }
	}

    
    return 0;
}