#include <iostream>
#include <duckdb/parser/parser.hpp>
#include <duckdb/parser/query_node/select_node.hpp>
#include <duckdb/parser/statement/update_statement.hpp>
#include <sys/stat.h>

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

	    cout << "========" << " Query Location: " << statement.stmt_location << "========" << endl;

	    switch (stmt->type) {
	        case StatementType::SELECT_STATEMENT: {
	        	auto &st = stmt->Cast<SelectStatement>();
	        	QueryNode &node = *st.node;

	        	cout << "Select Statement: " << endl;

	        	if(node.type == QueryNodeType::SELECT_NODE) {
	        		auto &node_select = dynamic_cast<SelectNode &>(node);


	        		cout << "Columns: ";

	        		//iterate through select list
	        		for(const auto &select_element : node_select.select_list) {
	        			cout << select_element->ToString() << " ";
	        		}

	        		cout << endl;

	        		cout << "Select From: " << node_select.from_table->ToString() << endl;
	        	}
	            break;
	        }
	        case StatementType::UPDATE_STATEMENT: {
	            auto &st = stmt->Cast<UpdateStatement>();

	        	auto &set_info = *st.set_info;

	        	cout << "Update Statement" << endl;

	        	cout << "Update Elements: " ;

	        	for(const auto &update_element : set_info.columns) {
	        		cout << update_element << " ";
	        	}

	        	cout << endl;

	        	cout << "Select From: " << st.table->ToString() << endl;
	        	break;
	        }
		    default: cout << "Unknown statement" << endl;
	    }
	}

    return 0;
}