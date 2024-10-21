#include <string>
#include <nebula/parser/nodes/set_operation_node.hpp>

namespace nebula
{
std::string SetOperationNode::ToString()
{
    std::string output = "Set Operation Node\nLeft\n";
    output += left->ToString() + "\nRight\n" + right->ToString();

    return output;
}
}
