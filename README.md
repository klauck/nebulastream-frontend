# NebulaStream SQL Parser

This parser library converts SQL strings (e.g., select name from students where id=42) to SQL statement objects, for which members (e.g., tables in the from clause) can be directly accessed.
The parser code is based on (extracted from) DuckDB (https://github.com/duckdb/duckdb/tree/main/third_party/libpg_query), which in turn is based on the PostgreSQL parser (https://github.com/pganalyze/libpg_query).

## Using Nebula SQL Parser

**Note:** You can also find a detailed usage description [here](docs/README.md).

To use the Nebula SQL Parser, follow these steps:

1. **Clone the Repository**  
   Start by cloning the Nebula SQL Parser repository:

   ```sh
   git clone https://github.com/nebulastream/sql-parser.git
   ```

2. **Create a New Directory**  
   Create a new directory for your project that will use the Nebula SQL Parser. Inside this directory, add a `CMakeLists.txt` file and a `main.cpp` file:

   ```shell
   mkdir my_project
   cd my_project
   touch CMakeLists.txt main.cpp
   ```

3. **Configure CMakeLists.txt**  
   In the `CMakeLists.txt` file, link the Nebula SQL Parser library by adding the following lines of code:

   ```cmake
   cmake_minimum_required(VERSION 3.29)
   project(example)

   add_executable(example main.cpp)
   target_link_libraries(example nebula_parser)
   ```

4. **Build the Project**  
   Use the following commands to build your project and run the executable:

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ./path/to/folder/example
   ```

## Example Program

Complete Example Project [Here](https://github.com/nebulastream/sql-parser/tree/usage_example/example)

```c++
//create parser instance
nebula::Parser parser;
//parse query
auto is_query_parsed = parser.parse(query);
//check if the query has been parsed or not
if(!is_query_parsed)
{/*if query not parsed*/}
//get statements
auto& statements = parser.statements;
//get size of the statements
auto& statements_count = statements.size();
//iterate over statements
for(auto& stmt : statements)
{
    //check if the type of the statement is select statement
    if(stmt->type == nebula::StatementType::SELECT_STATEMENT)
    {
        //down case the sql statement into select statement
        auto& select_statement = stmt->Cast<nebula::SelectStatement>();
        auto& query_node = select_statement.node;
        //check the type of query node
        if(query_node->type == nebula::QueryNodeType::SELECT_NODE)
        {
            //down cast query node into select node
            auto& select_node = query_node->Cast<nebula::SelectNode>();
            //get tables from select node
            auto& tables = select_node.from_tables;
            for(auto& table : tables)
            {
                //check type of the table
                if(table->type == nebula::TableReferenceType::BASE_TABLE)
                {
                    //down cast table into base table
                    auto& base_table = table->Cast<nebula::BaseTableRef>();
                    //table name: base_table.table_name
                }
            }
        }
    }
}
```

## Build Process

```
mkdir build
cd build
cmake ..
make
```

## Running Tests

```
cd nebulastream_tests/parser_tests
./parser_tests
```

## Formatting

We use clang-format for code formatting based on the NebulaStream style.

To format changed files, run `git clang-format`.

To format existing files, run `clang-format -i FILE`.
