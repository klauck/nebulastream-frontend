project(tpch_benchmark)

add_executable(main main.cpp tpch_queries.hpp
        tpch_queries.cpp)

target_link_libraries(main pg_query)