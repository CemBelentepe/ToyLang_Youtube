#include "Interpreter.h"

#include <iostream>
#include <sstream>

std::string Interpreter::makeError(std::string str, Token tok)
{
    std::stringstream ss;
    ss << "[ERROR] " << str << " [line: " << tok.line << "]\n";
    return ss.str();
}

Interpreter::Interpreter(std::unique_ptr<Expr>& root)
    : root(root)
{
}

int Interpreter::run()
{
    try
    {
        Value result = root->accept(this);
        std::cout << result << std::endl;
        return 0;
    }
    catch(const std::string& err)
    {
        std::cout << err;
        return 1;
    }
}

Value Interpreter::visit(ExprBinary* expr)
{
    Value lhs = expr->lhs->accept(this);
    Value rhs = expr->rhs->accept(this);
    if(lhs.tag == TypeTag::NUMBER && rhs.tag == TypeTag::NUMBER)
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
        default:
            throw makeError("Invalid operand for two numbers", expr->op);
        }
    }
    else if(lhs.tag == TypeTag::STRING && rhs.tag == TypeTag::STRING)
    {
        if(expr->op.type == TokenType::PLUS)
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
        if(rhs.tag == TypeTag::BOOL)
            return !std::get<bool>(rhs.data);
        else
            throw makeError("Invalid operand for operator '!'", expr->op);
    case TokenType::MINUS:
        if(rhs.tag == TypeTag::NUMBER)
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