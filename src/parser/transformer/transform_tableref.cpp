// duckdb reference: src/parser/transform/expression/transform_tableref.cpp
#include <memory>
#include <string>
#include <vector>
#include <nebula/common/exception.hpp>
#include <nebula/parser/refs/sub_query_ref.hpp>
#include <nebula/parser/refs/table_ref.hpp>
#include <nebula/parser/transformer/transformer.hpp>

namespace nebula
{
std::unique_ptr<TableRef> Transformer::TransformRangeVar(pgquery::PGRangeVar* range)
{
    auto result = std::make_unique<BaseTableRef>();
    result->table_name = range->relname;

    return std::move(result);
}

std::unique_ptr<TableRef> Transformer::TransformSubSelect(pgquery::PGRangeSubselect& root)
{
    Transformer subquery_transformer(*this);
    pgquery::PGSelectStmt select_stmt = PGCast<pgquery::PGSelectStmt>(*root.subquery);
    auto subquery = subquery_transformer.TransformSelectStatement(select_stmt);
    if (!subquery)
    {
        return nullptr;
    }
    auto result = std::make_unique<SubQueryRef>(std::move(subquery));
    if (root.alias)
    {
        result->alias = root.alias->aliasname;
    }

    return std::move(result);
}


std::unique_ptr<TableRef> Transformer::TransformTableRefNode(pgquery::PGNode* node)
{
    switch (node->type)
    {
        case pgquery::T_PGRangeVar: {
            auto range_var = reinterpret_cast<pgquery::PGRangeVar*>(node);
            return TransformRangeVar(range_var);
        }
        case pgquery::T_PGJoinExpr:
            throw NotImplementedException("Join expression not supported");
        case pgquery::T_PGRangeSubselect: {
            auto sub_select = PGCast<pgquery::PGRangeSubselect>(*node);
            return TransformSubSelect(sub_select);
        }
        case pgquery::T_PGRangeFunction:
            throw NotImplementedException("Range function not supported");
        case pgquery::T_PGPivotExpr:
            throw NotImplementedException("Pivot expression not supported");
        default:
            throw NotImplementedException("From Type %d not supported");
    }
}

std::unique_ptr<TableRef> Transformer::TransformFromClause(pgquery::PGList* from)
{
    std::unique_ptr<std::string> from_table = std::make_unique<std::string>();

    //if query has from clause
    if (from)
    {
        if (from->length == 0)
        {
            throw NotImplementedException("From clause is empty");
        }

        if (from->length > 1)
        {
            throw NotImplementedException("From clause has more than one table");
        }

        //nebula don't support multiple tables from
        auto n = static_cast<pgquery::PGNode*>(from->head->data.ptr_value);

        return std::move(TransformTableRefNode(n));
    }

    return nullptr;
}
}
