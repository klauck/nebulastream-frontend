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

std::string SelectNode::ToString()
{
    std::string output = "SELECT_NODE\n\tFROM\n";
    for (auto& tr : from_tables)
    {
        output += "\t\t" + tr->ToString() + "\n";
    }

    output += "\tCOLUMNS:\n";

    for (auto& expr : select_list)
    {
        output += "\t\t" + expr->ToString() + "\n";
    }

    if (where_clause != nullptr)
        output += "\tWHERE:\n\t\t" + where_clause->ToString();

    if (modifiers.size() > 0)
    {
        output += "\tMODIFIER:\n";
        for (auto& mod : modifiers)
        {
            output += "\t\t" + mod->ToString() + "\n";
        }
    }

    if (orders.size() != 0)
    {
        output += "\tORDER:\n";
        for (auto& order : orders)
        {
            output += order->ToString() + "\n";
        }
    }

    if (group_by != nullptr)
    {
        output += "\tGROUP:\n" + group_by->ToString();
    }

    return output;
}
}
