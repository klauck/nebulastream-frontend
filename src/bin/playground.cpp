#include <iostream>
#include <postgres_parser.hpp>
#include <duckdb/parser/parser.hpp>
#include <duckdb/parser/query_node/select_node.hpp>
#include <duckdb/parser/statement/update_statement.hpp>
#include <nodes/parsenodes.hpp>
#include <list>
#include <duckdb/common/serializer/serializer.hpp>
#include <duckdb/common/tree_renderer/json_tree_renderer.hpp>
#include <duckdb/execution/operator/helper/physical_result_collector.hpp>
#include <duckdb/execution/operator/schema/physical_create_table.hpp>
#include <duckdb/main/client_context.hpp>
#include <duckdb/main/connection.hpp>
#include <duckdb/main/database.hpp>
#include <duckdb/optimizer/optimizer.hpp>
#include <duckdb/planner/binder.hpp>
#include <duckdb/planner/planner.hpp>
#include <parser/parser.hpp>
#include <vector>
#include <duckdb/main/attached_database.hpp>
#include <duckdb/parser/parsed_data/create_schema_info.hpp>

using namespace duckdb;
using namespace std;

int PRINT_LOGS = 0;

#pragma region DuckDB Parser
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
#pragma endregion

#pragma region Postgress Parser

template<class T>

static T &PGCast(duckdb_libpgquery::PGNode &node) {
    return reinterpret_cast<T &>(node);
}

static list<string> ParseColumnRef(duckdb_libpgquery::PGColumnRef &ref) {
    auto head_node = static_cast<duckdb_libpgquery::PGNode *>(ref.fields->head->data.ptr_value);

    std::list<string> column_names;

    if (head_node->type == duckdb_libpgquery::T_PGString) {
        for (auto node = ref.fields->head; node; node = node->next) {
            auto value = static_cast<duckdb_libpgquery::PGValue *>(node->data.ptr_value);
            column_names.emplace_back(value->val.str);
        }
    } else {
        if (PRINT_LOGS)
            cout << "Unknown column reference" << endl;
    }

    return column_names;
}

static list<string> ParseStarRef(duckdb_libpgquery::PGAStar &ref) {
    return std::list<string>{"*"};
}

static list<string> ParseColumn(duckdb_libpgquery::PGNode &node) {
    switch (node.type) {
        //if the target type is response target
        case duckdb_libpgquery::T_PGResTarget: {
            if (PRINT_LOGS)
                cout << "Column is PGResTarget" << endl;
            const auto target = PGCast<duckdb_libpgquery::PGResTarget>(node);
            return ParseColumn(*target.val);
        }

        //if the node type is column reference
        case duckdb_libpgquery::T_PGColumnRef: {
            if (PRINT_LOGS)
                cout << "Column is PGColumnRef" << endl;
            auto column_ref = PGCast<duckdb_libpgquery::PGColumnRef>(node);
            return ParseColumnRef(column_ref);
        }

        // if the node is a *
        case duckdb_libpgquery::T_PGAStar: {
            if (PRINT_LOGS)
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

    //iterate through all columns
    for (auto node = list.head; node != nullptr; node = node->next) {
        const auto target = static_cast<duckdb_libpgquery::PGNode *>(node->data.ptr_value);
        //parse single column
        auto new_columns = ParseColumn(*target);

        for (const auto &c: new_columns) {
            columns.emplace_back(c);
        }
    }

    return columns;
}

static int ParsePGStatement(duckdb_libpgquery::PGNode *node) {
    switch (node->type) {
        //check if node type is select statement
        case duckdb_libpgquery::T_PGSelectStmt: {
            cout << "======Select Statement======" << endl;

            //casting node to select statement
            const auto cs = PGCast<duckdb_libpgquery::PGSelectStmt>(*node);

            //check if there is no union, intercept, etc in the statement
            if (cs.op == duckdb_libpgquery::PG_SETOP_NONE) {
                //getting from table clause
                const auto &from = cs.fromClause;

                //if query has from clause
                if (from) {
                    //iterate through all from clauses
                    for (auto node = from->head; node != nullptr; node = node->next) {
                        auto n = static_cast<duckdb_libpgquery::PGNode *>(node->data.ptr_value);

                        //check if the node type is range variable (used for from table clause)
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

                //if the node type is result target (select list)
                if (target->type == duckdb_libpgquery::T_PGResTarget) {
                    //parse all select columns
                    auto columns = ParseColumns(*targetList);
                    cout << "Columns: ";
                    for (const auto &col: columns) {
                        cout << col << ", ";
                    }
                    cout << endl;

                    return 1;
                }
            } else {
                cout << "Process not implemented for this statement" << endl;
                return 0;
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

    cout << "Query: " << query << endl;


    PostgresParser parser{};
    parser.Parse(query);

    if (parser.success && parser.parse_tree) {
        cout << "Query Parsed" << endl;
    } else {
        cout << "Unable to parse query." << endl;
        return -1;
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
    return 1;
}

#pragma endregion

#pragma region Binder
static void DuckDbBinderTest() {
    DuckDB db(nullptr);
    Connection con(db);

    //con.Query()
    auto &database_instance = db.instance;
    auto context = make_shared_ptr<ClientContext>(database_instance->shared_from_this());
    auto binder = Binder::CreateBinder(*context);
    Planner planner(*context);
    //Optimizer optimizer(binder, context);

    const auto create_query = "CREATE TABLE students(id INTEGER, name VARCHAR, age INTEGER);";
    const auto insert_query = "INSERT INTO students VALUES (1, 'Alice', 22), (2, 'Bob', 25), (3, 'Charlie', 23);";
    const auto select_query = "SELECT * FROM students;";

    Parser parser;
    parser.ParseQuery(create_query);

    const auto &statements = parser.statements;

    if (statements.empty()) {
        cout << "Unable to parse statements";
        return;
    }

    const duckdb::unique_ptr<SQLStatement> &create_sql_statement = statements[0];

    context->transaction.BeginTransaction();
    BoundStatement bounded_create_statement = binder->Bind(*create_sql_statement);
}
#pragma endregion

#pragma region Planner

int PrintPlan(const LogicalOperator &plan) {
    JSONTreeRenderer renderer{};
    const auto result = renderer.ToString(plan);

    cout << result << endl;

    return 0;
}

int PlannerTest(const string &query) {
    //init database
    DuckDB db(nullptr);

    //init db connection
    Connection con(db);

    //execute the query so that we can create bound statement without exceptions
    con.Query(query);

    auto &database_instance = db.instance;
    //create a database context, in our case it's a in memory database
    const auto context = make_shared_ptr<ClientContext>(database_instance->shared_from_this());
    //create planner instance
    Planner planner(*context);
    Parser parser;

    //parse query
    parser.ParseQuery(query);

    //check if the statements are successfully parsed
    if (parser.statements.empty()) {
        cout << "Unable to parse statements";
        return -1;
    }

    //create plan for each statement
    for (auto &statement: parser.statements) {
        cout << "Planning statement: " << statement->ToString() << endl;
        auto stmt = move(statement);

        //start a transaction
        context->transaction.BeginTransaction();
        //create plan using planner
        planner.CreatePlan(move(stmt));

        //get generated plan
        auto &plan = planner.plan;

        //verify plan
        Planner::VerifyPlan(*context, plan);
        cout << "Plan Verified" << endl;

        cout << "Plan: ";
        //print plan
        PrintPlan(*plan);

        //check if optimization is required
        if (plan->RequireOptimizer()) {
            cout << "Optimization Required" << endl;

            //init optimizer
            Optimizer optimizer(*planner.binder, *context);
            //optimize the plan using optimizer
            plan = optimizer.Optimize(std::move(plan));

            //verify plan
            Planner::VerifyPlan(*context, plan);
            cout << "New Plan Verified" << endl;

            cout << "New Plan: ";
            //print new plan
            PrintPlan(*plan);
        }


        PhysicalPlanGenerator physical_planner(*context);

        const auto &physical_plan = physical_planner.CreatePlan(move(plan));

        if (physical_plan->type == PhysicalOperatorType::CREATE_TABLE) {
            cout << "Create Table" << endl;
            const auto &physical_create_plan = physical_plan->Cast<PhysicalCreateTable>();
            const auto &catalog = physical_create_plan.schema.catalog;

            // auto schemas = catalog.GetAllSchemas(*context);
            //
            // for (const auto &schema: schemas) {
            //
            // }
        }
        // const auto &catalog = physical_plan-
        // auto &result_collector = pysical_plan->Cast<PhysicalResultCollector>();
        //
        // auto res = result_collector.GetResult(*result_collector.sink_state);
        //
        // cout << res << endl;
        //physical_plan->sche

        context->transaction.Commit();

        cout << "=======End of Query======" << endl;
        break;
    }

    return 0;
}
#pragma endregion

#pragma region Catalog
int CatalogTest(const string &query) {
    cout << "======Testing Catalog======" << endl;
    DuckDB db(nullptr);
    auto &database_instance = db.instance;
    auto context = make_shared_ptr<ClientContext>(database_instance->shared_from_this());

    //starting a transaction
    context->transaction.BeginTransaction();

    //create attached database instance from main db instance
    AttachedDatabase attached_database(*db.instance);
    //get the catalog from the attached database
    auto &catalog = Catalog::GetCatalog(attached_database);

    //get catalog transaction using active context
    const auto &catalog_trans = catalog.GetCatalogTransaction(*context);

    //create new schema information
    CreateSchemaInfo info;
    info.schema = "TEST SCHEMA";
    info.internal = true;

    //create schema in the catalog
    catalog.CreateSchema(catalog_trans, info);

    //commit transaction so that test schema being reflected in the catalog schemas
    context->transaction.Commit();

    context->transaction.BeginTransaction();
    //get schema from catalog
    const auto schemas = catalog.GetSchemas(*context);
    cout << "Schemas: " << endl;
    for (const auto &schema: schemas) {
        cout << "===" << schema.get().name << "===" << endl;
        const auto sql = schema.get().ToSQL();

        cout << "SQL: " << sql << endl;
        cout << "===" << "END SCHEMA" << "===" << endl << endl;
    }


    context->transaction.Commit();
    return 0;
}
#pragma endregion

int main() {
    const std::string query =
            "CREATE TABLE X(abc INT);SELECT abc, def, ghq FROM nebula_data, xyz;update nebula_data set a = 1 where b = 2;";
    const std::string test_query =
            "CREATE TABLE employees (id INT, name VARCHAR, department VARCHAR, salary DOUBLE, hire_date DATE, role VARCHAR); INSERT INTO employees VALUES (1, 'Alice', 'HR', 55000, '2020-01-15', 'Admin'), (2, 'Bob', 'Engineering', 70000, '2019-06-12', 'Other'), (3, 'Carol', 'HR', 60000, '2021-03-18', 'Admin'), (4, 'Dave', 'Sales', 48000, '2018-11-24', 'Other');";

    const std::string test_query_2 =
            "SELECT DISTINCT e.employee_id AS emp_id, e.first_name AS fname, e.last_name AS lname, d.department_name AS dept_name FROM employees AS e INNER JOIN departments AS d ON e.department_id = d.department_id WHERE d.department_name = 'Sales' ORDER BY e.last_name ASC;";

    ParseUsingDuckDB(test_query_2);
    //ParseUsingPGParser(query);
    //DuckDbBinderTest();
    //PlannerTest(test_query);
    //CatalogTest(test_query);

    return 0;
}
