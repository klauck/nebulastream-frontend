#include "nebula/parser/nodes/order_by_node.hpp"
#include <string>

namespace nebula
{
std::string OrderByNode::ToString()
{
    std::string output = "\t\tTYPE: " + OrderTypeToString(type) + "\n\t\tNULL ORDER: " + OrderByNullToString(null_order)
        + "\n\t\tExpressions:\n\t\t" + expression->ToString() + "\n";
    return output;
}

}
