#pragma once

#include "../expression.h"
#include "../statement.h"

// For a for loop statement.
class ASTStatementFor : public ASTStatement
{

    // Condition to check.
    std::unique_ptr<ASTStatement> initialization;
    std::unique_ptr<ASTExpression> condition;
    std::unique_ptr<ASTStatement> increment;
    // Then statement to execute.
    std::unique_ptr<ASTStatement> thenStatement;

public:
    // Create a new for statement.
    // condition: Condition to check.
    // thenStatement: Statement to execute for the condition is true.
    ASTStatementFor(std::unique_ptr<ASTStatement> initialization, std::unique_ptr<ASTExpression> condition, std::unique_ptr<ASTStatement> increment, std::unique_ptr<ASTStatement> thenStatement) : initialization(std::move(initialization)), condition(std::move(condition)), increment(std::move(increment)), thenStatement(std::move(thenStatement)) {}

    // Create a new if statement.
    // condition: Condition to check.
    // thenStatement: Statement to execute for the condition is true.
    static auto Create(std::unique_ptr<ASTStatement> initialization, std::unique_ptr<ASTExpression> condition, std::unique_ptr<ASTStatement> increment, std::unique_ptr<ASTStatement> thenStatement)
    {
        return std::make_unique<ASTStatementFor>(std::move(initialization), std::move(condition), std::move(increment), std::move(thenStatement));
    }

    // Virtual functions. See base class for details.
    virtual std::unique_ptr<VarType> StatementReturnType(ASTFunction &func) override;
    virtual void Compile(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func) override;
    virtual std::string ToString(const std::string &prefix) override;
};