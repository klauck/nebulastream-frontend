// duckdb reference: src/include/duckdb/common/enums/order_type.hpp
#pragma once
#include <string>

namespace nebula
{
enum class OrderType : short
{
    INVALID = 0,
    ORDER_DEFAULT = 1,
    ASCENDING = 2,
    DESCENDING = 3
};

enum class OrderByNullType : short
{
    INVALID = 0,
    ORDER_DEFAULT = 1,
    NULLS_FIRST = 2,
    NULLS_LAST = 3
};

enum class DefaultOrderByNullType : short
{
    INVALID = 0,
    NULLS_FIRST = 2,
    NULLS_LAST = 3,
    NULLS_FIRST_ON_ASC_LAST_ON_DESC = 4,
    NULLS_LAST_ON_ASC_FIRST_ON_DESC = 5
};

inline std::string OrderTypeToString(OrderType type)
{
    switch (type)
    {
        case OrderType::INVALID:
            return "INVALID";
        case OrderType::ORDER_DEFAULT:
            return "ORDER_DEFAULT";
        case OrderType::ASCENDING:
            return "ASCENDING";
        case OrderType::DESCENDING:
            return "DESCENDING";
        default:
            return "UNKNOWN";
    }
}

inline std::string OrderByNullToString(OrderByNullType type)
{
    switch (type)
    {
        case OrderByNullType::INVALID:
            return "INVALID";
        case OrderByNullType::ORDER_DEFAULT:
            return "ORDER_DEFAULT";
        case OrderByNullType::NULLS_FIRST:
            return "NULLS_FIRST";
        case OrderByNullType::NULLS_LAST:
            return "NULLS_LAST";
        default:
            return "UNKNOWN";
    }
}

inline std::string DefaultOrderByToString(DefaultOrderByNullType type)
{
    switch (type)
    {
        case DefaultOrderByNullType::INVALID:
            return "INVALID";
        case DefaultOrderByNullType::NULLS_FIRST:
            return "NULLS_FIRST";
        case DefaultOrderByNullType::NULLS_LAST:
            return "NULLS_LAST";
        case DefaultOrderByNullType::NULLS_FIRST_ON_ASC_LAST_ON_DESC:
            return "NULLS_FIRST_ON_ASC_LAST_ON_DESC";
        case DefaultOrderByNullType::NULLS_LAST_ON_ASC_FIRST_ON_DESC:
            return "NULLS_LAST_ON_ASC_FIRST_ON_DESC";
        default:
            return "UNKNOWN";
    }
}
}
