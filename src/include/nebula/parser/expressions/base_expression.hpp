#include<iostream>

namespace nebula {
    class BaseExpression {
        public: 
            BaseExpression() = default;
            virtual ~BaseExpression() = default;
            virtual void Print() = 0;

            std::unique_ptr<std::string> alias;
    };
}