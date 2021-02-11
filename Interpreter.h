#pragma once

#include <string>
#include "AstVisitor.hpp"

class Interpreter : public AstVisitor
{
private:
    std::unique_ptr<Expr>& root;
    std::string makeError(std::string str, Token tok);
public:
    Interpreter(std::unique_ptr<Expr>& root);
    int run();

    Value visit(ExprBinary* expr) override;
    Value visit(ExprUnary* expr) override;
    Value visit(ExprLiteral* expr) override;
};