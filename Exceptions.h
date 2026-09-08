#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

// Thrown by a BoxState when a requested lifecycle transition is not valid
// from the current state. Callers are expected to catch this and decide how
// to respond (log, retry, abort the operation) rather than the program
// crashing or silently doing nothing
class InvalidStateTransition : public std::exception {
    public:
        explicit InvalidStateTransition(const std::string& msg) : message_(msg) {}

        const char* what() const noexcept override { return message_.c_str(); }

    private:
        std::string message_;
};

#endif