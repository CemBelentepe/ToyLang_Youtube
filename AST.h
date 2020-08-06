#pragma once

#include "Scanner.h"
#include "Value.hpp"

#include <memory>

class AstVisitor;

class Expr
{
public:
    virtual void accept(AstVisitor* visitor) = 0;
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
    
    void accept(AstVisitor* visitor);
};

class ExprUnary : public Expr
{
public:
    std::unique_ptr<Expr> rhs;
    Token op;

    ExprUnary(std::unique_ptr<Expr> rhs, Token op)
        : rhs(std::move(rhs)), op(op)
    {}
    
    void accept(AstVisitor* visitor);
};

class ExprLiteral : public Expr
{
public:
    std::unique_ptr<Value> value;

    ExprLiteral(std::unique_ptr<Value> value)
        : value(std::move(value))
    {}

    void accept(AstVisitor* visitor);
};
