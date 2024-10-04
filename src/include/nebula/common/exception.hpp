#pragma once
#include <exception>
#include <string>

namespace nebula {
    class NotImplementedException : public std::logic_error {
    public:
        NotImplementedException(const std::string msg): std::logic_error(msg) {
        }
    };

    class InvalidOperationException : public std::logic_error {
    public:
        InvalidOperationException(const std::string msg): std::logic_error(msg) {
        }
    };
}
