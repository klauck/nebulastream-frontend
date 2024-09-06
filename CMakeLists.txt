cmake_minimum_required(VERSION 3.29)
project(nes_frontend)

set(CMAKE_CXX_STANDARD "11")

# functions defination

function(disable_target_warnings NAME)
  if(MSVC)
    target_compile_options(${NAME} PRIVATE "/W0")
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang$"
         OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${NAME} PRIVATE "-w")
  endif()
endfunction()

function(add_library_unity NAME MODE)
  set(SRCS ${ARGN})
#   if(NOT DISABLE_UNITY)
#     enable_unity_build(${NAME} SRCS)
#   endif()
  add_library(${NAME} OBJECT ${SRCS})
endfunction()

include_directories(src/include)
add_subdirectory(src)
add_subdirectory(third_party)



# include_directories(src/bin)

add_executable(playground src/bin/playground.cpp)

target_link_libraries(playground duckdb_pg_query)
target_link_libraries(playground duckdb_parser)
target_link_libraries(playground duckdb_utf8proc)
target_link_libraries(playground duckdb_common)
target_link_libraries(playground duckdb_fmt)