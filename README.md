# NebulaStream SQL Parser

This parser library converts SQL strings (e.g., select name from students where id=42) to SQL statement objects, for which members (e.g., tables in the from clause) can be directly accessed.
The parser code is based on (extracted from) DuckDB (https://github.com/duckdb/duckdb/tree/main/third_party/libpg_query), which in turn is based on the PostgreSQL parser (https://github.com/pganalyze/libpg_query).

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

