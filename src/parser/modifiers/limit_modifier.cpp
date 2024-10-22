#include <nebula/parser/modifiers/limit_modifier.hpp>
namespace nebula
{
std::string LimitModifier::ToString()
{
    std::string output;
    if (limit != 0)
    {
        output += "LIMIT " + std::to_string(limit) + ", ";
        output += "OFFSET " + std::to_string(offset) + "\n";
    }
    else
    {
        output += "OFFSET " + std::to_string(offset) + "\n";
    }

    return output;
}
}
