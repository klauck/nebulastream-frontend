//
// Created by Usama Bin Tariq on 21.09.24.
//
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
