#pragma once

#include <string>
#include "AstVisitor.hpp"

class Interpreter : public AstVisitor
{
private:
    std::vector<std::unique_ptr<Stmt>>& root;
    std::string makeError(std::string str, Token tok);
public:
    Interpreter(std::vector<std::unique_ptr<Stmt>>& root);
    int run();

    Value visit(ExprBinary* expr) override;
    Value visit(ExprUnary* expr) override;
    Value visit(ExprLiteral* expr) override;

    void visit(StmtExpr* expr) override;
    void visit(StmtBlock* stmt) override;
    void visit(StmtIf* stmt) override;
    void visit(StmtWhile* stmt) override;

    bool isTruthy(Value& val);
};