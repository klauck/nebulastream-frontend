set(CMAKE_CXX_STANDARD "14")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(example)

add_executable(example example.cpp)

target_link_libraries(example nebula_parser)
