#pragma once
#include "AST.h"

class AstVisitor
{
public:
    virtual Value visit(ExprBinary* expr) = 0;
    virtual Value visit(ExprUnary* expr) = 0;
    virtual Value visit(ExprLiteral* expr) = 0;


    virtual void visit(StmtExpr* stmt) = 0;
    virtual void visit(StmtBlock* stmt) = 0;
    virtual void visit(StmtIf* stmt) = 0;
    virtual void visit(StmtWhile* stmt) = 0;
};