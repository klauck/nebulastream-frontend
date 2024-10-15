// duckdb reference: src/parser/query_node/select_node.cpp
#include "nebula/parser/nodes/select_node.hpp"
#include <memory>
#include <string>
#include <vector>
#include "nebula/parser/nodes/query_node.hpp"

namespace nebula
{
SelectNode::SelectNode() : QueryNode(QueryNodeType::SELECT_NODE)
{
}
}
