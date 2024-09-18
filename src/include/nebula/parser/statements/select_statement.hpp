//
// Created by Usama Bin Tariq on 16.09.24.
//

#ifndef SELECT_STATEMENT_H
#define SELECT_STATEMENT_H
#include <string>
#include "sql_statement.hpp"

#endif //SELECT_STATEMENT_H

namespace nebula {
    class SelectStatement : public SQLStatement {
    public:
        void Print() const override;
        std::string ToString() const override;
        std::string ToStreamQuery() const override;
    };
 }