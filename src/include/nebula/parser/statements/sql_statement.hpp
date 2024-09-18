//
// Created by Usama Bin Tariq on 17.09.24.
//
#include <iostream>

#ifndef SQL_STATEMENT_HPP
#define SQL_STATEMENT_HPP

namespace nebula {
    class SQLStatement {
    public:
        virtual ~SQLStatement() = default;
        virtual std::string ToString() const = 0;
        virtual std::string ToStreamQuery() const = 0;
        virtual void Print() const = 0;
    };
}

#endif //SQL_STATEMENT_HPP
