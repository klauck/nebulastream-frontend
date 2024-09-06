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


include_directories(third_party/re2)
include_directories(src/include)
include_directories(third_party/utf8proc/include)
include_directories(third_party/fmt/include)
include_directories(third_party/fsst)
include_directories(third_party/miniz)
include_directories(third_party/pcg)
include_directories(third_party/jaro_winkler)
include_directories(third_party/yyjson/include)
add_subdirectory(src)
add_subdirectory(third_party)


# include_directories(src/bin)

add_executable(playground src/bin/playground.cpp)

target_link_libraries(playground duckdb_pg_query)
target_link_libraries(playground duckdb_parser)
target_link_libraries(playground duckdb_utf8proc)
target_link_libraries(playground duckdb_common)
target_link_libraries(playground duckdb_fmt)
target_link_libraries(playground duckdb_fsst)
target_link_libraries(playground duckdb_miniz)
target_link_libraries(playground duckdb_re2)
target_link_libraries(playground duckdb_function)