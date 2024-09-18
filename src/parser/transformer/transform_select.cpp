//
// Created by Usama Bin Tariq on 18.09.24.
//

#include <nebula/parser/transformer/transformer.hpp>
#include <iostream>
#include "../../libraries/pg_query/include/nodes/primnodes.hpp"
#include "../../libraries/pg_query/include/nodes/value.hpp"

namespace nebula {
    static void ParseColumnRef(pgquery::PGColumnRef &ref, std::unique_ptr<std::vector<std::string> > &columns) {
        auto head_node = static_cast<pgquery::PGNode *>(ref.fields->head->data.ptr_value);

        if (head_node->type == pgquery::T_PGString) {
            for (auto node = ref.fields->head; node; node = node->next) {
                const auto value = static_cast<pgquery::PGValue *>(node->data.ptr_value);
                columns->push_back(value->val.str);
            }
        }
    }

    static void ParseColumn(pgquery::PGNode &node, std::unique_ptr<std::vector<std::string> > &columns) {
        switch (node.type) {
            //if the target type is response target
            case pgquery::T_PGResTarget: {
                const auto target = Transformer::PGCast<pgquery::PGResTarget>(node);
                ParseColumn(*target.val, columns);
                break;
            }

            //if the node type is column reference
            case pgquery::T_PGColumnRef: {
                auto column_ref = Transformer::PGCast<pgquery::PGColumnRef>(node);
                ParseColumnRef(column_ref, columns);
                break;
            }

            // if the node is a *
            case pgquery::T_PGAStar: {
                columns->push_back("*");
                break;
            }
            default: {
            };
        }
    }

    std::unique_ptr<SelectStatement> Transformer::TransformSelectStatement(pgquery::PGSelectStmt &statement) {
        std::unique_ptr<SelectStatement> result = std::make_unique<SelectStatement>();

        if (statement.op == pgquery::PG_SETOP_NONE) {
            //getting from table clause
            const auto &from = statement.fromClause;

            //if query has from clause
            if (from) {
                //iterate through all from clauses
                for (auto node = from->head; node != nullptr; node = node->next) {
                    auto n = static_cast<pgquery::PGNode *>(node->data.ptr_value);

                    //check if the node type is range variable (used for from table clause)
                    if (n->type == pgquery::T_PGRangeVar) {
                        auto range_root = static_cast<pgquery::PGRangeVar *>(node->data.ptr_value);
                        result->from->push_back(range_root->relname);
                    }
                }
            }

            //get columns list
            auto &targetList = statement.targetList;

            auto target = static_cast<pgquery::PGNode *>(targetList->head->data.ptr_value);

            //if the node type is result target (select list)
            if (target->type == pgquery::T_PGResTarget) {
                //parse all select columns
                for (auto node = targetList->head; node != nullptr; node = node->next) {
                    auto target = static_cast<pgquery::PGNode *>(node->data.ptr_value);
                    //parse single column
                    ParseColumn(*target, result->columns);
                }
            }
        }

        return result;
    }
}
