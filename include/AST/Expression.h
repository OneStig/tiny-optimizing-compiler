#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include "ASTNode.h"

namespace AST {
    class Expression : public ASTNode {
    public:
        Expression() = default;
        std::vector<bool> negate; // starting after first child
    };

    class Term : public ASTNode {
    public:
        Term() = default;
        std::vector<bool> divide; // starting after first child
    };

    class Factor : public ASTNode {
    public:
        Factor() = default;
        std::string ident;
        int value{};
    };
}

#endif //EXPRESSION_H
