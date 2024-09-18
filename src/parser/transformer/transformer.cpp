//
// Created by Usama Bin Tariq on 18.09.24.
//

#include<iostream>
#include "nebula/parser/transformer/transformer.hpp"

namespace nebula {
    bool Transformer::TransformParseTree(pgquery::PGList *tree, std::unique_ptr<SQLStatementCollection> &collection) {
        for (auto entry = tree->head; entry != nullptr; entry = entry->next) {
            auto n = static_cast<pgquery::PGNode *>(entry->data.ptr_value);
            auto stmt = TransformStatement(*n);
            collection->add_statement(stmt);
        }
        return true;
    }

    std::unique_ptr<SQLStatement> Transformer::TransformStatement(pgquery::PGNode &node) {
        return TransformStatementInternal(node);
    }

    std::unique_ptr<SQLStatement> Transformer::TransformStatementInternal(pgquery::PGNode &node) {
        switch (node.type) {
            case pgquery::T_PGRawStmt: {
                auto &raw_stmt = PGCast<pgquery::PGRawStmt>(node);
                auto result = TransformStatement(*raw_stmt.stmt);
                return result;
            }
            case pgquery::T_PGSelectStmt:
                return TransformSelectStatement(PGCast<pgquery::PGSelectStmt>(node));
            default: {
                std::cout << "TransformSelectStatement: unknown type" << std::endl;
                return nullptr;
            }
        }
    }

    Transformer Transformer::clone() {
        return Transformer();
    }
} // nebula
