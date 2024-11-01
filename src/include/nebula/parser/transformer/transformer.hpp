// duckdb reference: src/include/duckdb/parser/transformer.hpp
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <nebula/parser/nodes/order_by_node.hpp>
#include <nebula/parser/statements/select_statement.hpp>
#include <parser/gramparse.hpp>

namespace nebula
{
class Transformer
{
public:
    Transformer() = default;

    virtual ~Transformer() = default;

    virtual Transformer clone();

    virtual bool TransformParseTree(pgquery::PGList* tree, std::vector<std::unique_ptr<SQLStatement>>& statements);

    template <class T>
    static T& PGCast(pgquery::PGNode& node)
    {
        return reinterpret_cast<T&>(node);
    }

    template <class T>
    static std::unique_ptr<T> PGPointerCast(void* ptr)
    {
        return std::unique_ptr<T>(reinterpret_cast<T*>(ptr));
    }

private:
    virtual std::unique_ptr<SQLStatement> TransformStatement(pgquery::PGNode* node);

    virtual std::unique_ptr<SQLStatement> TransformStatementInternal(pgquery::PGNode* node);

    virtual std::unique_ptr<SelectStatement> TransformSelectStatement(pgquery::PGSelectStmt& statement);

    virtual std::unique_ptr<ParsedExpression> TransformExpression(pgquery::PGNode* node);

    virtual std::unique_ptr<ParsedExpression> TransformAExpression(pgquery::PGAExpr& root);

    virtual std::unique_ptr<ParsedExpression> TransformColumnRef(pgquery::PGColumnRef& root);

    virtual std::unique_ptr<ParsedExpression> TransformResTarget(pgquery::PGResTarget& target);

    virtual std::vector<std::unique_ptr<ParsedExpression>> TransformExpressionList(pgquery::PGList* list);

    virtual std::unique_ptr<ParsedExpression> TransformConstant(pgquery::PGAConst& con);

    virtual std::unique_ptr<ParsedExpression> TransformBool(pgquery::PGBoolExpr& expr);

    virtual std::unique_ptr<ParsedExpression>
    TransformBinaryOperator(std::string& op, std::unique_ptr<ParsedExpression> left, std::unique_ptr<ParsedExpression> right);

    virtual void TransformResultModifiers(pgquery::PGSelectStmt& root, QueryNode& node);
    virtual std::unique_ptr<ParsedExpression> TransformFunction(pgquery::PGFuncCall& func);

    virtual std::unique_ptr<TableRef> TransformSubSelect(pgquery::PGRangeSubselect& root);

    std::unique_ptr<QueryNode> TransformSelectNode(pgquery::PGSelectStmt& statement);

    virtual std::unique_ptr<TableRef> TransformRangeVar(pgquery::PGRangeVar* range);

    virtual std::unique_ptr<TableRef> TransformTableRefNode(pgquery::PGNode* node);

    std::vector<std::unique_ptr<TableRef>> TransformFromClause(pgquery::PGList* from);

    std::unique_ptr<GroupByNode> TransformGroupBy(pgquery::PGList* from);

    std::vector<std::unique_ptr<OrderByNode>> TransformOrderBy(pgquery::PGList* order);
};
}
