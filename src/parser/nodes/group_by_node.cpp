#include "nebula/parser/nodes/group_by_node.hpp"
#include <string>

namespace nebula
{
std::string GroupByNode::ToString()
{
    std::string output;
    for (auto& e : group_expressions)
    {
        output += "\t\t" + e->ToString() + "\n";
    }

    return output;
}
}
