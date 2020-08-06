#include "Parser.h"
#include "AST.h"
#include "Value.hpp"

Parser::Parser(std::vector<Token>& tokens)
    : tokens(tokens), currentToken(0)
{}

std::unique_ptr<Expr> Parser::parse()
{
    return std::move(parseExpr());
}

std::unique_ptr<Expr> Parser::parseExpr()
{
    return std::move(addition());
}

std::unique_ptr<Expr> Parser::addition()
{
    std::unique_ptr<Expr> lhs = multiplication();
    while(match({TokenType::PLUS, TokenType::MINUS}))
    {
        Token op = consumed();
        std::unique_ptr<Expr> rhs = multiplication();
        lhs = std::make_unique<ExprBinary>(std::move(lhs), std::move(rhs), op);
    }

    return std::move(lhs);
}

std::unique_ptr<Expr> Parser::multiplication()
{
    std::unique_ptr<Expr> lhs = unary();
    while(match({TokenType::STAR, TokenType::SLASH}))
    {
        Token op = consumed();
        std::unique_ptr<Expr> rhs = unary();
        lhs = std::make_unique<ExprBinary>(std::move(lhs), std::move(rhs), op);
    }

    return std::move(lhs);
}

std::unique_ptr<Expr> Parser::unary()
{
    if(match({TokenType::MINUS, TokenType::BANG}))
    {
        Token op = consumed();
        std::unique_ptr<Expr> expr = unary();
        expr = std::make_unique<ExprUnary>(std::move(expr), op);
        return std::move(expr);
    }
    else
        return std::move(primary());
}

std::unique_ptr<Expr> Parser::primary()
{
    Token token = advance();

    switch(token.type)
    {
    case TokenType::TRUE:
        return std::make_unique<ExprLiteral>(std::make_unique<Value>(true));
    case TokenType::FALSE:
        return std::make_unique<ExprLiteral>(std::make_unique<Value>(false));
    case TokenType::NUMBER_LITERAL:
        return std::make_unique<ExprLiteral>(std::make_unique<Value>(token.getNumber()));
    case TokenType::STRING_LITERAL:
        return std::make_unique<ExprLiteral>(std::make_unique<Value>(token.getString()));
    case TokenType::OPEN_PAREN:
    {
        std::unique_ptr<Expr> expr = parseExpr();
        if(match(TokenType::CLOSE_PAREN))
            return std::move(expr);
        else
            return errorAtToken("Expect ')' after a grouping expression.");
    }
    default:
        return errorAtToken("Invalid identifier.");
    }
}

void Parser::panic()
{
    TokenType type = tokens[currentToken].type;
    while (type != TokenType::EOF_TOKEN)
    {
        if (type == TokenType::SEMI_COLON)
        {
            advance();
            return;
        }

        switch (type)
        {
        case TokenType::CLASS:
        case TokenType::IF:
        case TokenType::VAR:
        case TokenType::FUNC:
        case TokenType::WHILE:
        case TokenType::FOR:
        case TokenType::RETURN:
            return;

        default:
            break;
        }

        type = advance().type;
    }
}
