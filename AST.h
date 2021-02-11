#pragma once

#include "Scanner.h"
#include "Value.hpp"

#include <memory>

class AstVisitor;

class Expr
{
public:
    virtual Value accept(AstVisitor* visitor) = 0;
    virtual ~Expr(){}
};

class ExprBinary : public Expr
{
public:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
    Token op;

    ExprBinary(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs, Token op)
        : lhs(std::move(lhs)), rhs(std::move(rhs)), op(op)
    {}
    
    Value accept(AstVisitor* visitor) override;
};

class ExprUnary : public Expr
{
public:
    std::unique_ptr<Expr> rhs;
    Token op;

    ExprUnary(std::unique_ptr<Expr> rhs, Token op)
        : rhs(std::move(rhs)), op(op)
    {}
    
    Value accept(AstVisitor* visitor) override;
};

class ExprLiteral : public Expr
{
public:
    Value value;

    ExprLiteral(Value value)
        : value(value)
    {}

    Value accept(AstVisitor* visitor) override;
};
