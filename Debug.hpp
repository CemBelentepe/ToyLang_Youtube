#pragma once
#include "AstVisitor.hpp"

#include <iostream>

class AstDebugger : public AstVisitor
{
private:
    Expr* root;
public:
    AstDebugger(Expr* root)
        : root(root)
    {}

    void debug()
    {
        root->accept(this);
        std::cout << "\n";
    }

    // (op, lhs, rhs)
    void visit(ExprBinary* expr)
    {
        std::cout << "(" << expr->op.getString() << ", ";
        expr->lhs->accept(this);
        std::cout << ", ";
        expr->rhs->accept(this);
        std::cout << ")";
    }

    // (op rhs)
    void visit(ExprUnary* expr)
    {
        std::cout << "(" << expr->op.getString();
        expr->rhs->accept(this);
        std::cout << ")";
    }

    void visit(ExprLiteral* expr)
    {
        expr->value->print();
    }
};