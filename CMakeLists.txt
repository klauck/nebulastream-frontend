cmake_minimum_required(VERSION 3.29)
project(nes_frontend)

set(CMAKE_CXX_STANDARD "11")

include_directories(src/include)
add_subdirectory(third_party)

# include_directories(src/bin)

add_executable(playground src/bin/playground.cpp)

target_link_libraries(playground duckdb_pg_query)
