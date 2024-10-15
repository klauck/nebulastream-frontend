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
#include <nebula/parser/nodes/set_operation_node.hpp>

namespace nebula {
    void Transformer::TransformResultModifiers(pgquery::PGSelectStmt &stmt, QueryNode &node) {
        //if either of the limit is set
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
            } else {
                throw NotImplementedException("Percentage limit is not implemented");
            }
        }
    }

    std::unique_ptr<QueryNode> Transformer::TransformSelectNode(pgquery::PGSelectStmt &statement) {
        std::unique_ptr<QueryNode> query_node;

        switch (statement.op) {
            case pgquery::PG_SETOP_NONE: {
                query_node = std::make_unique<SelectNode>();
                auto &select_node = query_node->Cast<SelectNode>();
                select_node.from_table = TransformFromClause(statement.fromClause);
                select_node.select_list = TransformExpressionList(statement.targetList);
                select_node.where_clause = TransformExpression(statement.whereClause);
                break;
            }
            case pgquery::PG_SETOP_UNION: {
                query_node = std::make_unique<SetOperationNode>();
                auto &result = query_node->Cast<SetOperationNode>();
                //FIXME: Implement with clause
                result.left = TransformSelectNode(*statement.larg);
                result.right = TransformSelectNode(*statement.rarg);

                switch (statement.op) {
                    case pgquery::PG_SETOP_UNION:
                        result.setop_type = SetOperationType::UNION;
                        break;
                    case pgquery::PG_SETOP_EXCEPT:
                        result.setop_type = SetOperationType::EXCEPT;
                        break;
                    case pgquery::PG_SETOP_INTERSECT:
                        result.setop_type = SetOperationType::INTERSECT;
                        break;
                    case pgquery::PG_SETOP_UNION_BY_NAME:
                        result.setop_type = SetOperationType::UNION_BY_NAME;
                        break;
                    default:
                        throw InvalidOperationException("Operator is not implemented");
                }

                break;
            }
            default: {
                throw NotImplementedException("Select expression not supported");
            }
        }

        TransformResultModifiers(statement, *query_node);
        return std::move(query_node);
    }

    std::unique_ptr<SelectStatement> Transformer::TransformSelectStatement(pgquery::PGSelectStmt &statement) {
        std::unique_ptr<SelectStatement> result = std::make_unique<SelectStatement>();

        result->node = TransformSelectNode(statement);
        return result;
    }
}
