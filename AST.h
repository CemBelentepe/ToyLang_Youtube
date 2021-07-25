#pragma once

#include "Scanner.h"
#include "Value.hpp"

#include <memory>

class AstVisitor;

enum class ExprType 
{
    Binary, Unary, Literal, VarGet, VarSet
};

class Expr
{
public:
    ExprType instance;

    Expr(ExprType instance)
        : instance(instance) {}
    virtual ~Expr() = default;
    virtual Value accept(AstVisitor* visitor) = 0;
};

class ExprBinary : public Expr
{
public:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
    Token op;

    ExprBinary(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs, Token op)
        : Expr(ExprType::Binary), lhs(std::move(lhs)), rhs(std::move(rhs)), op(op)
    {
    }

    Value accept(AstVisitor* visitor) override;
};

class ExprUnary : public Expr
{
public:
    std::unique_ptr<Expr> rhs;
    Token op;

    ExprUnary(std::unique_ptr<Expr> rhs, Token op)
        : Expr(ExprType::Unary), rhs(std::move(rhs)), op(op)
    {
    }

    Value accept(AstVisitor* visitor) override;
};

class ExprLiteral : public Expr
{
public:
    Value value;

    ExprLiteral(Value value)
        : Expr(ExprType::Literal), value(value)
    {
    }

    Value accept(AstVisitor* visitor) override;
};

class ExprVarGet : public Expr
{
public:
    Token name;

    ExprVarGet(Token name)
        :Expr(ExprType::VarGet), name(name)
    {}

    Value accept(AstVisitor* visitor) override;
};

class ExprVarSet : public Expr
{
public:
    Token name;
    std::unique_ptr<Expr> assgn;
    Token op;

    ExprVarGet(Token name, std::unique_ptr<Expr> assgn, Token op)
        :Expr(ExprType::VarSet), name(name), assgn(std::move(assgn)), op(op)
    {}

    Value accept(AstVisitor* visitor) override;
};

// Statements
class Stmt
{
public:
    virtual void accept(AstVisitor* visitor) = 0;
    virtual ~Stmt() = default;
};

class StmtExpr : public Stmt
{
public:
    std::unique_ptr<Expr> expr;

    StmtExpr(std::unique_ptr<Expr> expr)
        : expr(std::move(expr)) {}

    void accept(AstVisitor* visitor) override;
};

class StmtBlock : public Stmt
{
public:
    std::vector<std::unique_ptr<Stmt>> stmts;

    StmtBlock(std::vector<std::unique_ptr<Stmt>> stmts)
        : stmts(std::move(stmts)) {}

    void accept(AstVisitor* visitor) override;
};

class StmtIf : public Stmt
{
public:
    std::unique_ptr<Expr> cond;
    std::unique_ptr<Stmt> if_;
    std::unique_ptr<Stmt> els;

    StmtIf(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> if_, std::unique_ptr<Stmt> els)
        : cond(std::move(cond)), if_(std::move(if_)), els(std::move(els)) {}

    void accept(AstVisitor* visitor) override;
};

class StmtWhile : public Stmt
{
public:
    std::unique_ptr<Expr> cond;
    std::unique_ptr<Stmt> body;

    StmtWhile(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> body)
        : cond(std::move(cond)), body(std::move(body)){}

    void accept(AstVisitor* visitor) override;
};

class StmtVarDecl : public Stmt
{
public:
    Token name;
	std::unique_ptr<Expr> initVal;

	StmtVarDecl(Token name, std::unique_ptr<Expr> initVal = nullptr)
		: name(name), initVal(std::move(initVal))
	{}

	void accept(StmtVisitor* visitor) override;
}
