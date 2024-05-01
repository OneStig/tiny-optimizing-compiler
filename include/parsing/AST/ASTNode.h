#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include <memory>
#include "../../synthesis/IRBuilder.h"

namespace AST {

    class ASTNode;
    using ASTPtr = std::unique_ptr<ASTNode>;

    class ASTNode {
    public:
        std::vector<ASTPtr> children;

        virtual ~ASTNode() = default;

        void append(ASTPtr node) {
            children.push_back(std::move(node));
        }

        virtual int evaluate(IRBuilder& builder, int& block) {
            for (const ASTPtr &child: children) {
                child->evaluate(builder, block);
            }

            return 0;
        };
    };


}

#endif //ASTNODE_H
