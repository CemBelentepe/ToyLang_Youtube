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

void StmtExpr::accept(AstVisitor* visitor)
{
    return visitor->visit(this);
}

void StmtBlock::accept(AstVisitor* visitor)
{
    return visitor->visit(this);
}

void StmtIf::accept(AstVisitor* visitor)
{
    return visitor->visit(this);
}

void StmtWhile::accept(AstVisitor* visitor)
{
    return visitor->visit(this);
}