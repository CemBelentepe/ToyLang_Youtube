#include "AST.h"
#include "AstVisitor.hpp"

void ExprBinary::accept(AstVisitor* visitor)
{
    visitor->visit(this);
}

void ExprUnary::accept(AstVisitor* visitor)
{
    visitor->visit(this);
}

void ExprLiteral::accept(AstVisitor* visitor)
{
    visitor->visit(this);
}