#include "parsing/AST/Expressions.h"

int AST::Expression::evaluate(IRBuilder& builder, int& block) {
    int curSSA = children[0]->evaluate(builder, block);

    for (int i = 1; i < children.size(); i++) {
        const int nextSSA = children[i]->evaluate(builder, block);
        const InsType type = negate[i - 1] ? InsType::SUB : InsType::ADD;
        curSSA = builder.emit(block, type, curSSA, nextSSA);
    }

    return curSSA;
}

int AST::Term::evaluate(IRBuilder& builder, int& block) {
    int curSSA = children[0]->evaluate(builder, block);

    for (int i = 1; i < children.size(); i++) {
        const int nextSSA = children[i]->evaluate(builder, block);
        const InsType type = divide[i - 1] ? InsType::DIV : InsType::MUL;
        curSSA = builder.emit(block, type, curSSA, nextSSA);
    }

    return curSSA;
}

int AST::Factor::evaluate(IRBuilder& builder, int& block) {
    // return ssa value of 1st child regardless of num, ident, expr
    return children[0]->evaluate(builder, block);
}

int AST::Identifier::evaluate(IRBuilder& builder, int& block) {
    return builder.blocks[block].nameTable[name];
}

int AST::Number::evaluate(IRBuilder &builder, int& block) {
    return builder.emit(0, InsType::CONST, value);
}

int AST::Relation::evaluate(IRBuilder &builder, int& block) {
    return builder.emit(block, InsType::CMP,
        children[0]->evaluate(builder, block),
        children[1]->evaluate(builder, block));
}
