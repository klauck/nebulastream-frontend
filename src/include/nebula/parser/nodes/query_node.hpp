//duckdb reference: src/include/duckdb/parser/query_node.hpp

#pragma once
#include <vector>
#include <memory>
#include "nebula/parser/modifiers/result_modifier.hpp"
#include "nebula/common/exception.hpp"

namespace nebula {
    enum class QueryNodeType : short {
        SELECT_NODE = 1,
        SET_OPERATION_NODE = 2,
        BOUND_SUBQUERY_NODE = 3,
        RECURSIVE_CTE_NODE = 4,
        CTE_NODE = 5
    };

    class QueryNode {
    public:
        explicit QueryNode(QueryNodeType type) : type(type) {
        }

        virtual ~QueryNode() {
        }

        std::vector<std::unique_ptr<nebula::ResultModifier> > modifiers;

        QueryNodeType type;

        template<class TARGET>
        TARGET &Cast() {
            if (type != TARGET::TYPE) {
                throw InvalidOperationException(
                    "Failed to cast query node to type - query node type mismatch, ");
            }
            return reinterpret_cast<TARGET &>(*this);
        }
    };
}
