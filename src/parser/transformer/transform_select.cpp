//
// Created by Usama Bin Tariq on 18.09.24.
//

#include <nebula/parser/transformer/transformer.hpp>
#include <string>
#include <vector>
#include <memory>
#include <nebula/common/exception.hpp>
#include <nebula/parser/expressions/constant_expression.hpp>
#include <nebula/parser/modifiers/limit_modifier.hpp>

namespace nebula {
    void Transformer::TransformResultModifiers(pgquery::PGSelectStmt &stmt, QueryNode &node) {
        if (stmt.limitCount || stmt.limitOffset) {
            if (stmt.type != pgquery::T_PGLimitPercent) {
                auto limit_modifier = std::make_unique<LimitModifier>();

                //convert the limit value to parsed expression
                if (stmt.limitCount) {
                    auto limit_const_exp = Transformer::TransformExpression(stmt.limitCount);

                    if (limit_const_exp->type != ExpressionType::VALUE_CONSTANT) {
                        throw NotImplementedException("limit constant expression is not implemented");
                    }

                    auto const_expr = limit_const_exp->Cast<ConstantExpression>();

                    limit_modifier->limit = const_expr.value.asInt32();
                }

                if (stmt.limitOffset) {
                    auto limit_offset_const_exp = Transformer::TransformExpression(stmt.limitOffset);

                    if (limit_offset_const_exp->type != ExpressionType::VALUE_CONSTANT) {
                        throw NotImplementedException("limit constant expression is not implemented");
                    }

                    auto const_expr = limit_offset_const_exp->Cast<ConstantExpression>();

                    limit_modifier->offset = const_expr.value.asInt32();
                }

                node.modifiers.emplace_back(std::move(limit_modifier));
            }
        }
    }

    std::unique_ptr<SelectStatement> Transformer::TransformSelectStatement(pgquery::PGSelectStmt &statement) {
        std::unique_ptr<SelectStatement> result = std::make_unique<SelectStatement>();
        std::unique_ptr<SelectNode> select_node = std::make_unique<SelectNode>();

        if (statement.op == pgquery::PG_SETOP_NONE) {
            select_node->from_table = TransformFromClause(statement.fromClause);
            select_node->select_list = TransformExpressionList(statement.targetList);
            select_node->where_clause = TransformExpression(statement.whereClause);
        } else {
            throw NotImplementedException("Select expression not supported");
        }

        TransformResultModifiers(statement, *select_node);

        result->select_node = std::move(select_node);
        return std::move(result);
    }
}
