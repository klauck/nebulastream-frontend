//
// Created by Usama Bin Tariq on 18.09.24.
//

#pragma once
#include <nebula/parser/statements/select_statement.hpp>
#include <nebula/parser/statements/sql_statement_collection.hpp>
#include <parser/gramparse.hpp>

namespace nebula {
  class Transformer {
  public:
    Transformer() = default;

    virtual ~Transformer() = default;

    virtual Transformer clone();

    virtual bool TransformParseTree(pgquery::PGList *tree, std::unique_ptr<SQLStatementCollection> &collection);

    template<class T>
    static T &PGCast(pgquery::PGNode &node) {
      return reinterpret_cast<T &>(node);
    }

    template<class T>
    static std::unique_ptr<T> PGPointerCast(void *ptr) {
      return std::unique_ptr<T>(reinterpret_cast<T *>(ptr));
    }

  private:
    virtual std::unique_ptr<SQLStatement> TransformStatement(pgquery::PGNode &node);
    virtual std::unique_ptr<SQLStatement> TransformStatementInternal(pgquery::PGNode &node);
    virtual std::unique_ptr<SelectStatement> TransformSelectStatement(pgquery::PGSelectStmt &statement);
  };
}
