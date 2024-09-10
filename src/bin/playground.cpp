#include <iostream>
#include <postgres_parser.hpp>
#include <duckdb/parser/parser.hpp>
#include <duckdb/parser/query_node/select_node.hpp>
#include <duckdb/parser/statement/update_statement.hpp>
#include <nodes/parsenodes.hpp>
#include <list>

using namespace duckdb;
using namespace std;

int PRINT_LOGS = 0;

static int ParseUsingDuckDB(const string &query) {
    cout << "Query: " << query << endl;

    Parser parser;

    try {
        parser.ParseQuery(query);
    } catch (std::exception &e) {
        cout << "Exception: " << e.what() << endl;
        return -1;
    }

    cout << "Query Parsed" << endl;
    cout << "Query size: " << parser.statements.size() << endl;
    const auto &statements = parser.statements; // Use a const reference to avoid copying

    for (const auto &stmt: statements) {
        auto &statement = *stmt;

        cout << "========" << " Query Location: " << statement.stmt_location << "========" << endl;

        switch (stmt->type) {
            case StatementType::SELECT_STATEMENT: {
                auto &st = stmt->Cast<SelectStatement>();
                QueryNode &node = *st.node;

                cout << "Select Statement: " << endl;

                if (node.type == QueryNodeType::SELECT_NODE) {
                    auto &node_select = dynamic_cast<SelectNode &>(node);


                    cout << "Columns: ";

                    //iterate through select list
                    for (const auto &select_element: node_select.select_list) {
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

                cout << "Update Elements: ";

                for (const auto &update_element: set_info.columns) {
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

template<class T>

static T &PGCast(duckdb_libpgquery::PGNode &node) {
    return reinterpret_cast<T &>(node);
}

static list<string> ParseColumnRef(duckdb_libpgquery::PGColumnRef &ref) {
    auto head_node = static_cast<duckdb_libpgquery::PGNode*>(ref.fields->head->data.ptr_value);

    std::list<string> column_names;

    if(head_node->type == duckdb_libpgquery::T_PGString) {
        for (auto node = ref.fields->head; node; node = node->next) {
            auto value = static_cast<duckdb_libpgquery::PGValue*>(node->data.ptr_value);
            column_names.emplace_back(value->val.str);
        }
    }else {
        if(PRINT_LOGS)
            cout << "Unknown column reference" << endl;
    }

    return column_names;
}

static list<string> ParseStarRef(duckdb_libpgquery::PGAStar &ref) {
    return std::list<string>{"*"};
}

static list<string> ParseColumn(duckdb_libpgquery::PGNode &node) {
    switch (node.type) {
        case duckdb_libpgquery::T_PGResTarget: {
            if(PRINT_LOGS)
                cout << "Column is PGResTarget" << endl;
            const auto target = PGCast<duckdb_libpgquery::PGResTarget>(node);
            return ParseColumn(*target.val);
        }
        case duckdb_libpgquery::T_PGColumnRef: {
            if(PRINT_LOGS)
                cout << "Column is PGColumnRef" << endl;
            auto column_ref = PGCast<duckdb_libpgquery::PGColumnRef>(node);
            return ParseColumnRef(column_ref);
        }
        case duckdb_libpgquery::T_PGAStar: {
            if(PRINT_LOGS)
                cout << "Column is PGAStar" << endl;
            auto st = PGCast<duckdb_libpgquery::PGAStar>(node);
            return ParseStarRef(st);
        }
        default: {
            cout << "Unknown node type" << endl;
            return {};
        }
    }
}

static std::list<string> ParseColumns(const duckdb_libpgquery::PGList &list) {
    std::list<string> columns = {};

    for (auto node = list.head; node != nullptr; node = node->next) {
        const auto target = static_cast<duckdb_libpgquery::PGNode *>(node->data.ptr_value);
        auto new_columns = ParseColumn(*target);

        for(const auto& c: new_columns) {
            columns.emplace_back(c);
        }
    }

    return columns;
}

static int ParsePGStatement(duckdb_libpgquery::PGNode *node) {
    switch (node->type) {
        case duckdb_libpgquery::T_PGSelectStmt: {
            cout << "======Select Statement======" << endl;

            const auto cs = PGCast<duckdb_libpgquery::PGSelectStmt>(*node);

            if (cs.op == duckdb_libpgquery::PG_SETOP_NONE) {
                const auto &from = cs.fromClause;

                if (from) {
                    for (auto node = from->head; node != nullptr; node = node->next) {
                        auto n = static_cast<duckdb_libpgquery::PGNode *>(node->data.ptr_value);

                        if (n->type == duckdb_libpgquery::T_PGRangeVar) {
                            auto range_root = static_cast<duckdb_libpgquery::PGRangeVar *>(node->data.ptr_value);

                            if (range_root) {
                                cout << "From Table Name: " << range_root->relname << endl;
                            } else {
                                cout << "Range Root Not Found" << endl;
                            }
                        } else {
                            cout << "Unknown node type" << endl;
                        }
                    }
                } else {
                    cout << "No from clause" << endl;
                }

                //get columns list
                const auto &targetList = cs.targetList;

                auto target = static_cast<duckdb_libpgquery::PGNode *>(targetList->head->data.ptr_value);

                if (target->type == duckdb_libpgquery::T_PGResTarget) {
                    auto columns = ParseColumns(*targetList);
                    cout << "Columns: ";
                    for (const auto& col: columns) {
                        cout << col << ", ";
                    }
                    cout << endl;
                }
            } else {
                cout << "Process not implemented for this statement" << endl;
            }
            cout << "======END Select Statement======" << endl;
            break;
        }
        case duckdb_libpgquery::T_PGUpdateStmt: {
            cout << "Update Statement" << endl;
            break;
        }
        default: {
            cout << "Unknown statement: " << node->type << endl;
        };
    }

    return 0;
}

static int ParsePGRawStatement(const void *node) {
    const auto *raw_stmt = static_cast<const duckdb_libpgquery::PGRawStmt *>(node);

    return ParsePGStatement(raw_stmt->stmt);
}

static int ParseUsingPGParser(const string &query) {
    //ParserOptions options; //using default options

    PostgresParser parser{};

    //parse query
    parser.Parse(query);

    cout << "Query: " << query << endl;

    if (parser.success && parser.parse_tree) {
        cout << "Query Parsed" << endl;
    } else {
        cout << "Query Parsed" << endl;
    }

    const auto tree = parser.parse_tree;

    //each head is a sql statement, iterating through all statements
    for (auto entry = tree->head; entry != nullptr; entry = entry->next) {
        //casting the current node to PG node
        auto *node = static_cast<duckdb_libpgquery::PGNode *>(entry->data.ptr_value);

        if (node->type == duckdb_libpgquery::T_PGRawStmt) {
            //parse raw statement
            ParsePGRawStatement(entry->data.ptr_value);
        } else {
            //parse pg statement
            ParsePGStatement(node);
        }
    }
}

int main() {
    const std::string query = "SELECT abc, def, ghq FROM nebula_data;update nebula_data set a = 1 where b = 2;";

    //ParseUsingDuckDB(query);

    ParseUsingPGParser(query);

    return 0;
}
