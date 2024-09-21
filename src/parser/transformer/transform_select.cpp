//
// Created by Usama Bin Tariq on 18.09.24.
//

#include <nebula/parser/transformer/transformer.hpp>
#include <iostream>
#include <nebula/common/exception.hpp>
#include <nebula/parser/refs/table_ref.hpp>
#include <nebula/parser/refs/base_table_ref.hpp>
#include <nebula/parser/expressions/star_expression.hpp>
#include <nebula/parser/refs/table_ref_type.hpp>
#include <memory>
#include <nebula/parser/transformer/transformer.hpp>
#include<string>
#include <nebula/parser/expressions/column_expression.hpp>

#include "../../libraries/pg_query/include/nodes/primnodes.hpp"
#include "../../libraries/pg_query/include/nodes/value.hpp"

namespace nebula {
    static std::unique_ptr<ParsedExpression> ParseColumnRef(pgquery::PGColumnRef &ref) {
        auto result = std::make_unique<ColumnExpression>();
        auto head_node = static_cast<pgquery::PGNode *>(ref.fields->head->data.ptr_value);

        if (head_node->type == pgquery::T_PGString) {
            for (auto node = ref.fields->head; node; node = node->next) {
                const auto value = static_cast<pgquery::PGValue *>(node->data.ptr_value);
                result->column_names.push_back(value->val.str);
            }

            return result;
        }

        throw NotImplementedException("Parsing column ref failed");
    }

    static std::unique_ptr<ParsedExpression> ParseColumn(pgquery::PGNode &node) {
        switch (node.type) {
            //if the target type is response target
            case pgquery::T_PGResTarget: {
                const auto target = Transformer::PGCast<pgquery::PGResTarget>(node);
                return ParseColumn(*target.val);
            }

            //if the node type is column reference
            case pgquery::T_PGColumnRef: {
                auto column_ref = Transformer::PGCast<pgquery::PGColumnRef>(node);
                return ParseColumnRef(column_ref);
            }

            // if the node is a *
            case pgquery::T_PGAStar: {
                return std::make_unique<StarExpression>();
                break;
            }
            default: {
                throw NotImplementedException("Unsupported node type");
            };
        }
    }

    static std::unique_ptr<TableRef> TransformRangeVar(pgquery::PGRangeVar *range) {
        auto result = std::make_unique<TableRef>();
        result->table_name = range->relname;

        return std::move(result);
    }

    static std::unique_ptr<TableRef> TransformTableRefNode(pgquery::PGNode *node) {
        switch (node->type) {
            case pgquery::T_PGRangeVar: {
                auto range_var = reinterpret_cast<pgquery::PGRangeVar *>(node);
                return TransformRangeVar(range_var);
            }
            case pgquery::T_PGJoinExpr:
                throw NotImplementedException("Join expression not supported");
            case pgquery::T_PGRangeSubselect:
                throw NotImplementedException("Range subselect not supported");
            case pgquery::T_PGRangeFunction:
                throw NotImplementedException("Range function not supported");
            case pgquery::T_PGPivotExpr:
                throw NotImplementedException("Pivot expression not supported");
            default:
                throw NotImplementedException("From Type %d not supported");
        }
    }

    static std::unique_ptr<TableRef> TransformFromClause(pgquery::PGList *from) {
        std::unique_ptr<std::string> from_table = std::make_unique<std::string>();

        //if query has from clause
        if (from) {
            if (from->length == 0) {
                throw NotImplementedException("From clause is empty");
                return nullptr;
            }

            if (from->length > 1) {
                throw NotImplementedException("From clause has more than one table");
                return nullptr;
            }

            auto n = static_cast<pgquery::PGNode *>(from->head->data.ptr_value);
            //auto x = static_cast<pgquery::PGRangeVar *>(from->head->data.ptr_value);


            return std::move(TransformTableRefNode(n));
        }

        return nullptr;
    }

    static std::vector<std::unique_ptr<ParsedExpression> > TransformerTargetList(pgquery::PGList *targetList) {
        auto result = std::vector<std::unique_ptr<ParsedExpression> >();

        for (auto node = targetList->head; node; node = node->next) {
            const auto column = static_cast<pgquery::PGNode *>(node->data.ptr_value);

            if (column->type == pgquery::T_PGResTarget) {
                auto expression = ParseColumn(*column);
                result.push_back(std::move(expression));
            } else {
                throw NotImplementedException("Unsupported node type");
            }
        }

        return std::move(result);
    }

    std::unique_ptr<SelectStatement> Transformer::TransformSelectStatement(pgquery::PGSelectStmt &statement) {
        std::unique_ptr<SelectStatement> result = std::make_unique<SelectStatement>();
        std::unique_ptr<SelectNode> select_node = std::make_unique<SelectNode>();

        if (statement.op == pgquery::PG_SETOP_NONE) {
            select_node->from_table = TransformFromClause(statement.fromClause);
            select_node->select_list = TransformerTargetList(statement.targetList);
        } else {
            throw NotImplementedException("Select expression not supported");
        }

        return result;
    }
}
