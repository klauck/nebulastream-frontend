project(parser_tests)

enable_testing()

add_executable(parser_tests simple_select_query.cpp
        select_query_with_where_test.cpp
        all_tests.cpp
        query_parsing_test.cpp
        select_limit_test.cpp
        union_query_test.cpp
        postgres_query_parsing_test.cpp
        subquery_test.cpp)

target_link_libraries(parser_tests nebula_parser)
target_link_libraries(parser_tests pg_query)
target_link_libraries(parser_tests GTest::gtest_main)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sql_files/queries.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sql_files/hyrise_queries.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
