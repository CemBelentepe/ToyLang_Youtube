#include "AST.h"

class AstVisitor
{
public:
    virtual void visit(ExprBinary* expr) = 0;
    virtual void visit(ExprUnary* expr) = 0;
    virtual void visit(ExprLiteral* expr) = 0;
};