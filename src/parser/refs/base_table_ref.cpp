#include <string>
#include <nebula/parser/refs/base_table_ref.hpp>

namespace nebula
{
std::string BaseTableRef::ToString()
{
    if (alias.empty())
        return table_name;

    return table_name + " (" + alias + ")";
}
}
