project(nebula_nodes)

add_library(nebula_nodes OBJECT select_node.cpp
        set_operation_node.cpp
        group_by_node.cpp
        order_by_node.cpp)
