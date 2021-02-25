#include "Parser.h"
#include "AST.h"
#include "Value.hpp"

Parser::Parser(std::vector<Token>& tokens)
    : tokens(tokens), currentToken(0), hadError(false)
{
}

std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
    std::vector<std::unique_ptr<Stmt>> root;
    while (!match(TokenType::EOF_TOKEN))
    {
        root.push_back(statement()); // TODO change to decleration
    }

    return std::move(root);
}

std::unique_ptr<Stmt> Parser::statement()
{
    TokenType next = peek().type;

    switch (next)
    {
    case TokenType::OPEN_BRACE:
        return blockStmt();
    case TokenType::IF:
        return ifStmt();
    case TokenType::WHILE:
        return whileStmt();
    default:
        return exprStmt();
    }
}

std::unique_ptr<Stmt> Parser::exprStmt()
{
    std::unique_ptr<Expr> expr = parseExpr();
    consume(TokenType::SEMI_COLON, "Expect ';' after an expression statement.");
    return std::make_unique<StmtExpr>(std::move(expr));
}

std::unique_ptr<Stmt> Parser::blockStmt()
{
    advance();
    std::vector<std::unique_ptr<Stmt>> body;
    while(peek().type != TokenType::EOF_TOKEN && peek().type != TokenType::CLOSE_BRACE)
    {
        body.push_back(statement());
    }
    consume(TokenType::CLOSE_BRACE, "Expect '}' after a block statement.");
    return std::make_unique<StmtBlock>(std::move(body));
}

std::unique_ptr<Stmt> Parser::ifStmt()
{
    advance();
	consume(TokenType::OPEN_PAREN, "Expect '(' after 'if'.");

	std::unique_ptr<Expr> cond = parseExpr();
	consume(TokenType::CLOSE_PAREN, "Expect ')' after if condition.");

    std::unique_ptr<Stmt> if_ = statement();
    std::unique_ptr<Stmt> els = nullptr;
    if(match(TokenType::ELSE))
    {
        els = statement();
    }

    return std::make_unique<StmtIf>(std::move(cond), std::move(if_), std::move(els));
}

std::unique_ptr<Stmt> Parser::whileStmt()
{
    advance();
	consume(TokenType::OPEN_PAREN, "Expect '(' after 'while'.");

	std::unique_ptr<Expr> cond = parseExpr();
	consume(TokenType::CLOSE_PAREN, "Expect ')' after while condition.");

    std::unique_ptr<Stmt> body = statement();

    return std::make_unique<StmtWhile>(std::move(cond), std::move(body));
}

std::unique_ptr<Expr> Parser::parseExpr()
{
    return equality();
}

std::unique_ptr<Expr> Parser::equality()
{
	std::unique_ptr<Expr> lhs = comparison();
	while (match({ TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL }))
	{
		Token op = consumed();
		std::unique_ptr<Expr> rhs = comparison();
		lhs = std::make_unique<ExprBinary>(std::move(lhs), std::move(rhs), op);
	}

	return std::move(lhs);
}

std::unique_ptr<Expr> Parser::comparison()
{
	std::unique_ptr<Expr> lhs = addition();
	while (match({ TokenType::LESS, TokenType::GREAT, TokenType::LESS_EQUAL, TokenType::GREAT_EQUAL }))
	{
		Token op = consumed();
		std::unique_ptr<Expr> rhs = addition();
		lhs = std::make_unique<ExprBinary>(std::move(lhs), std::move(rhs), op);
	}

	return std::move(lhs);
}

std::unique_ptr<Expr> Parser::addition()
{
    std::unique_ptr<Expr> lhs = multiplication();
    while (match({TokenType::PLUS, TokenType::MINUS}))
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
    while (match({TokenType::STAR, TokenType::SLASH}))
    {
        Token op = consumed();
        std::unique_ptr<Expr> rhs = unary();
        lhs = std::make_unique<ExprBinary>(std::move(lhs), std::move(rhs), op);
    }

    return std::move(lhs);
}

std::unique_ptr<Expr> Parser::unary()
{
    if (match({TokenType::MINUS, TokenType::BANG}))
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

    switch (token.type)
    {
    case TokenType::TRUE:
        return std::make_unique<ExprLiteral>(Value(true));
    case TokenType::FALSE:
        return std::make_unique<ExprLiteral>(Value(false));
    case TokenType::NUMBER_LITERAL:
        return std::make_unique<ExprLiteral>(Value(token.getNumber()));
    case TokenType::STRING_LITERAL:
        return std::make_unique<ExprLiteral>(Value(token.getString()));
    case TokenType::OPEN_PAREN:
    {
        std::unique_ptr<Expr> expr = parseExpr();
        if (match(TokenType::CLOSE_PAREN))
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
    this->hadError = true;
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
