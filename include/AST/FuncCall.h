#ifndef FUNCCALL_H
#define FUNCCALL_H

#include <string>
#include "AST/Statement.h"

namespace AST {
    class FuncCall : public Statement { // children represent parameters
    public:
        std::string ident;
        FuncCall() = default;
    };
}


#endif //FUNCCALL_H
