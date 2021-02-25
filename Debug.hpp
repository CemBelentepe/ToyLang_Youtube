#pragma once
#include "AstVisitor.hpp"

#include <iostream>

class AstDebugger : public AstVisitor
{
private:
    std::vector<std::unique_ptr<Stmt>>& root;

public:
    AstDebugger(std::vector<std::unique_ptr<Stmt>>& root)
        : root(root)
    {
    }

    void debug()
    {
        for (auto& stmt : root)
        {
            stmt->accept(this);
            std::cout << "\n";
        }
    }

    // (op, lhs, rhs)
    Value visit(ExprBinary* expr) override
    {
        std::cout << "(" << expr->op.getString() << ", ";
        expr->lhs->accept(this);
        std::cout << ", ";
        expr->rhs->accept(this);
        std::cout << ")";
        return Value();
    }

    // (op rhs)
    Value visit(ExprUnary* expr) override
    {
        std::cout << "(" << expr->op.getString();
        expr->rhs->accept(this);
        std::cout << ")";
        return Value();
    }

    Value visit(ExprLiteral* expr) override
    {
        expr->value.print();
        return Value();
    }

    void visit(StmtExpr* stmt) override
    {
        stmt->expr->accept(this);
        std::cout << ";";
    }

    void visit(StmtBlock* stmt) override
    {
        std::cout << "{\n";
        for(auto& s : stmt->stmts)
        {
            s->accept(this);
            std::cout << "\n";
        }
        std::cout << "}";
    }

    void visit(StmtIf* stmt) override
    {
        std::cout << "IF ";
        stmt->cond->accept(this);
        std::cout << ":\n";
        stmt->if_->accept(this);
        if (stmt->els)
        {
            std::cout << "\nELSE:\n";
            stmt->els->accept(this);
        }
    }

    void visit(StmtWhile* stmt) override
    {
        std::cout << "While ";
        stmt->cond->accept(this);
        std::cout << ":\n";
        stmt->body->accept(this);
    }
};