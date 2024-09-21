cmake_minimum_required(VERSION 3.29)
project(nes_frontend)

set(CMAKE_CXX_STANDARD "14")

include_directories(src/include)
include_directories(src/libraries/pg_query/include)
add_subdirectory(src)

# include_directories(src/bin)

add_executable(playground src/bin/playground.cpp)

target_link_libraries(playground pg_query)
target_link_libraries(playground statements)
target_link_libraries(playground nebula_parser)
target_link_libraries(playground nebula_transformer)
target_link_libraries(playground nebula_nodes)
target_link_libraries(playground nebula_expressions)
