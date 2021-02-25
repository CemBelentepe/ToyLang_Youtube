#include "Interpreter.h"

#include <iostream>
#include <sstream>

std::string Interpreter::makeError(std::string str, Token tok)
{
    std::stringstream ss;
    ss << "[ERROR] " << str << " [line: " << tok.line << "]\n";
    return ss.str();
}

Interpreter::Interpreter(std::vector<std::unique_ptr<Stmt>>& root)
    : root(root)
{
}

int Interpreter::run()
{
    for (auto& stmt : root)
    {
        try
        {
            stmt->accept(this);
        }
        catch (const std::string& err)
        {
            std::cout << err;
            return 1;
        }
    }
    return 0;
}

Value Interpreter::visit(ExprBinary* expr)
{
    Value lhs = expr->lhs->accept(this);
    Value rhs = expr->rhs->accept(this);
    if(expr->op.type == TokenType::EQUAL_EQUAL)
    {
        return lhs.data == rhs.data;
    }
    else if(expr->op.type == TokenType::BANG_EQUAL)
    {
        return lhs.data != rhs.data;
    }
    else if (lhs.tag == TypeTag::NUMBER && rhs.tag == TypeTag::NUMBER)
    {
        switch (expr->op.type)
        {
        case TokenType::PLUS:
            return std::get<double>(lhs.data) + std::get<double>(rhs.data);
        case TokenType::MINUS:
            return std::get<double>(lhs.data) - std::get<double>(rhs.data);
        case TokenType::STAR:
            return std::get<double>(lhs.data) * std::get<double>(rhs.data);
        case TokenType::SLASH:
            return std::get<double>(lhs.data) / std::get<double>(rhs.data);
        case TokenType::LESS:
            return std::get<double>(lhs.data) < std::get<double>(rhs.data);
        case TokenType::GREAT:
            return std::get<double>(lhs.data) > std::get<double>(rhs.data);
        case TokenType::LESS_EQUAL:
            return std::get<double>(lhs.data) <= std::get<double>(rhs.data);
        case TokenType::GREAT_EQUAL:
            return std::get<double>(lhs.data) >= std::get<double>(rhs.data);
        default:
            throw makeError("Invalid operand for two numbers", expr->op);
        }
    }
    else if (lhs.tag == TypeTag::STRING && rhs.tag == TypeTag::STRING)
    {
        if (expr->op.type == TokenType::PLUS)
        {
            return std::get<std::string>(lhs.data) + std::get<std::string>(rhs.data);
        }
        else
        {
            throw makeError("Invalid operand for two strings", expr->op);
        }
    }
    else
    {
        throw makeError("Invalid type for binary operand", expr->op);
    }
    return Value();
}

Value Interpreter::visit(ExprUnary* expr)
{
    Value rhs = expr->rhs->accept(this);
    switch (expr->op.type)
    {
    case TokenType::BANG:
        if (rhs.tag == TypeTag::BOOL)
            return !std::get<bool>(rhs.data);
        else
            throw makeError("Invalid operand for operator '!'", expr->op);
    case TokenType::MINUS:
        if (rhs.tag == TypeTag::NUMBER)
            return -std::get<double>(rhs.data);
        else
            throw makeError("Invalid operand for operator '-'", expr->op);
        break;
    default:
        throw makeError("Invalid unary operand", expr->op);
    }
}

Value Interpreter::visit(ExprLiteral* expr)
{
    return expr->value;
}

void Interpreter::visit(StmtExpr* stmt)
{
    Value result = stmt->expr->accept(this);
    std::cout << result << std::endl;
}

void Interpreter::visit(StmtBlock* stmt)
{
    for (auto& s : stmt->stmts)
    {
        s->accept(this);
    }
}

void Interpreter::visit(StmtIf* stmt)
{
    Value cond = stmt->cond->accept(this);
    if (isTruthy(cond))
        stmt->if_->accept(this);
    else
        stmt->els->accept(this);
}

void Interpreter::visit(StmtWhile* stmt)
{
    Value cond = stmt->cond->accept(this);
    while (isTruthy(cond))
    {
        stmt->body->accept(this);
        cond = stmt->cond->accept(this);
    }
}

bool Interpreter::isTruthy(Value& val)
{
    if (val.tag == TypeTag::BOOL)
    {
        return std::get<bool>(val.data);
    }
    // TODO Add null check
    return true;
}