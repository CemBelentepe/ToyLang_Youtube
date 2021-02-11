#pragma once
#include "AST.h"

class AstVisitor
{
public:
    virtual Value visit(ExprBinary* expr) = 0;
    virtual Value visit(ExprUnary* expr) = 0;
    virtual Value visit(ExprLiteral* expr) = 0;
};