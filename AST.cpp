#include "AST.h"
#include "AstVisitor.hpp"

Value ExprBinary::accept(AstVisitor* visitor)
{
    return visitor->visit(this);
}

Value ExprUnary::accept(AstVisitor* visitor)
{
    return visitor->visit(this);
}

Value ExprLiteral::accept(AstVisitor* visitor)
{
    return visitor->visit(this);
}