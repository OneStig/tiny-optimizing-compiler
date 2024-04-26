#ifndef FUNCCALL_H
#define FUNCCALL_H

#include <string>
#include "AST/Statement.h"

namespace AST {
    class FuncCall : public Statement { // children represent parameters
    public:
        std::string ident;
        FuncCall() = default;

        int evaluate(IRBuilder& builder, int block) override {

            // Predefined functions

            if (ident == "InputNum") {
                return builder.emit(block, InsType::READ);
            }

            if (ident == "OutputNum") {
                return builder.emit(block, InsType::WRITE,
                    children[0]->evaluate(builder, block));
            }

            if (ident == "OutputNewLine") {
                return builder.emit(block, InsType::WRITENL);
            }


            return 0;
        }
    };
}


#endif //FUNCCALL_H
