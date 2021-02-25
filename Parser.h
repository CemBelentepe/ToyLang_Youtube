#pragma once

#include "Scanner.h"

#include <iostream>
#include <memory>
#include <vector>

class Expr;
class Stmt;

class Parser
{
private:
    std::vector<Token>& tokens;
    size_t currentToken;
    bool hadError;

public:
    Parser(std::vector<Token>& tokens);
    std::vector<std::unique_ptr<Stmt>> parse();
    bool fail() { return hadError; }

private:
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> exprStmt();
    std::unique_ptr<Stmt> blockStmt();
    std::unique_ptr<Stmt> ifStmt();
    std::unique_ptr<Stmt> whileStmt();

    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> addition();
    std::unique_ptr<Expr> multiplication();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    Token& advance()
    {
        return this->tokens[this->currentToken++];
    }
    Token& consumed() const
    {
        return this->tokens[this->currentToken - 1];
    }
    void consume(TokenType type, const std::string& msg)
    {
        if (tokens[currentToken].type == type)
        {
            advance();
        }
        else
        {
            errorAtToken(msg);
        }
    }
    Token& peek() const
    {
        return this->tokens[this->currentToken];
    }
    Token& peekNext() const
    {
        return this->tokens[this->currentToken + 1];
    }
    bool match(TokenType type)
    {
        TokenType next = tokens[currentToken].type;
        if (next == type)
        {
            currentToken++;
            return true;
        }

        return false;
    }
    bool match(std::vector<TokenType> types)
    {
        for (auto& token : types)
        {
            if (match(token))
                return true;
        }
        return false;
    }

    void panic();

    std::unique_ptr<Expr> error(const std::string& message)
    {
        std::cout << message << std::endl;
        this->panic();
        return nullptr;
    }

    std::unique_ptr<Expr> errorAtToken(const std::string& message)
    {
        std::cout << "[line" << tokens[currentToken].line << "] Error" << message << std::endl;
        this->panic();
        return nullptr;
    }
};