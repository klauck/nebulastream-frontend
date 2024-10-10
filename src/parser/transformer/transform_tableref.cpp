//
// Created by Usama Bin Tariq on 23.09.24.
//

#include <string>
#include <vector>
#include <memory>
#include <datatype/timestamp.hpp>
#include <nebula/common/exception.hpp>
#include <nebula/parser/refs/table_ref.hpp>
#include <nebula/parser/transformer/transformer.hpp>
#include <nebula/parser/refs/sub_query_ref.hpp>

namespace nebula {
    std::unique_ptr<TableRef> Transformer::TransformRangeVar(pgquery::PGRangeVar *range) {
        auto result = std::make_unique<BaseTableRef>();
        result->table_name = range->relname;

        return std::move(result);
    }

    std::unique_ptr<TableRef> Transformer::TransformSubSelect(pgquery::PGRangeSubselect &root) {
        Transformer subquery_transformer(*this);
        pgquery::PGSelectStmt select_stmt = PGCast<pgquery::PGSelectStmt>(*root.subquery);
        auto subquery = subquery_transformer.TransformSelectStatement(select_stmt);
        if (!subquery) {
            return nullptr;
        }
        auto result = std::make_unique<SubQueryRef>(std::move(subquery));
        if (root.alias) {
            result->alias = root.alias->aliasname;
        }

        return std::move(result);
    }


    std::unique_ptr<TableRef> Transformer::TransformTableRefNode(pgquery::PGNode *node) {
        switch (node->type) {
            case pgquery::T_PGRangeVar: {
                auto range_var = reinterpret_cast<pgquery::PGRangeVar *>(node);
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

    std::vector<std::unique_ptr<TableRef> > Transformer::TransformFromClause(pgquery::PGList *from,
                                                                             std::unique_ptr<WindowRangeNode> &
                                                                             window_node) {
        std::unique_ptr<std::string> from_table = std::make_unique<std::string>();


        //if query has from clause

        std::vector<std::unique_ptr<TableRef> > results;

        if (from) {
            if (from->length == 0) {
                throw NotImplementedException("From clause is empty");
            }

            for (auto cc = from->head; cc != nullptr; cc = cc->next) {
                auto *node = static_cast<pgquery::PGNode *>(cc->data.ptr_value);

                if (node->type == pgquery::T_PGWinRangeClause) {
                    auto range_clause = reinterpret_cast<pgquery::PGWinRangeClause *>(node);

                    auto interval = reinterpret_cast<pgquery::IntervalValue *>(range_clause->interval);

                    window_node = std::make_unique<WindowRangeNode>(WindowRangeNodeType::TUMBLING_WINDOW);
                    window_node->range = std::make_unique<RangeIntervalExpression>();
                    window_node->range->unit = interval->unit;

                    //convert value into pg const
                    auto iv = reinterpret_cast<pgquery::PGAConst *>(interval->value);

                    if (iv != nullptr) {
                        window_node->range->value = iv->val.val.ival;
                    }
                } else {
                    results.emplace_back(std::move(TransformTableRefNode(node)));
                }
            }
        }

        return results;
    }
}
