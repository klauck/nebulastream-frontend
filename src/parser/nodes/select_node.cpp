//duckdb reference: src/parser/query_node/select_node.cpp

#include "nebula/parser/nodes/select_node.hpp"
#include "nebula/parser/nodes/query_node.hpp"
#include <string>
#include <vector>
#include <memory>

namespace nebula {
    SelectNode::SelectNode(): QueryNode(QueryNodeType::SELECT_NODE) {
    }
}
