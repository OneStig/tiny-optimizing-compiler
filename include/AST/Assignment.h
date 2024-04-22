#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <string>
#include "AST/Statement.h"

namespace AST {
    class Assignment : public Statement {
    public:
        std::string ident;
        Assignment() = default;
    };
}

#endif //ASSIGNMENT_H
