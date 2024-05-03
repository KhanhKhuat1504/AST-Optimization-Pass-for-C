#pragma once

#include "../statement.h"
#include "../expression.h"

class ASTStatementBreak : public ASTStatement
{
public:
    void MyOptznPass(std::unique_ptr<ASTStatement> &parentPtr, ASTFunction &func) override {}

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> StatementReturnType(ASTFunction &func) override;
    void Compile(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func) override;
    std::string ToString(const std::string &prefix) override;
};