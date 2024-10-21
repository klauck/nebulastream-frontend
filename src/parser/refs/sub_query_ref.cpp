#include <string>
#include <nebula/parser/refs/sub_query_ref.hpp>

namespace nebula
{
std::string SubQueryRef::ToString()
{
    return subquery->ToString();
}

}
