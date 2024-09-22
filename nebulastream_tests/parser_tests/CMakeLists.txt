project(parser_tests)

enable_testing()

add_executable(parser_tests simple_select_query.cpp)

target_link_libraries(parser_tests nebula_parser)
target_link_libraries(parser_tests GTest::gtest_main)