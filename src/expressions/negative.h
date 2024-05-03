#pragma once

#include "../expression.h"

// An expression that adds two operands together.
class ASTExpressionNegation : public ASTExpression
{
    // Operands to work with.
    std::unique_ptr<ASTExpression> a;

    // Return type to cache.
    VarTypeSimple *returnType = nullptr;

public:
    // Create a new addition expression.
    // a1: Left side expression of the addition statement.
    // a2: Right side expression of the addition statement.
    ASTExpressionNegation(std::unique_ptr<ASTExpression> a) : a(std::move(a)) {}

    // Create a new addition expression.
    // a1: Left side expression of the addition statement.
    // a2: Right side expression of the addition statement.
    static auto Create(std::unique_ptr<ASTExpression> a)
    {
        return std::make_unique<ASTExpressionNegation>(std::move(a));
    }

    void MyOptznPass(std::unique_ptr<ASTExpression> &parentPtr, ASTFunction &func) override;

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction &func) override;
    bool IsLValue(ASTFunction &func) override;
    llvm::Value *Compile(llvm::IRBuilder<> &builder, ASTFunction &func) override;
    std::string ToString(const std::string &prefix) override;
};