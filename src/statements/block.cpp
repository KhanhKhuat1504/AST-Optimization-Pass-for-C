#include "block.h"

#include "../types/simple.h"

#include <iostream>

std::unique_ptr<VarType> ASTStatementBlock::StatementReturnType(ASTFunction &func)
{

    // This one is more interesting.
    // If we come across a statement that returns something, then we simply just return it.
    // Otherwise, we return nothing since we made it to the end.

    // Go through each statement.
    for (auto &statement : statements)
    {
        auto ret = statement->StatementReturnType(func);
        if (ret)
            return std::move(ret);
    }

    // Made it through the end, return nothing.
    return nullptr;
}

void ASTStatementBlock::MyOptznPass(std::unique_ptr<ASTStatement> &parentPtr, ASTFunction &func)
{

    for (std::unique_ptr<ASTStatement> &statement : statements)
    {
        statement->MyOptznPass(statement, func);
        if (statement->StatementReturnType(func))
            return;
    }

    int i = 0;
    while (i < statements.size())
    {
        ASTStatementBlock *blackStmt = dynamic_cast<ASTStatementBlock *>(statements[i].get());
        if (blackStmt)
        {
            std::cout << "block" << std::endl;
            for (int j = 0; j < blackStmt->statements.size(); j++)
            {
                statements.insert(statements.begin() + i + j, std::move(blackStmt->statements[j]));
            }
            statements.erase(statements.begin() + i + statements.size());
        }
        else
        {
            i++;
        }
    }

    // Block : stmt block stmt stmt stmt stmt stmt block stm
    //              ^
}

void ASTStatementBlock::Compile(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func)
{

    // Pretty much just keep compiling until we find a return value. We don't want to keep compiling past any return statements.
    for (auto &statement : statements)
    {
        statement->Compile(mod, builder, func);
        if (statement->StatementReturnType(func))
            return;
    }
}

std::string ASTStatementBlock::ToString(const std::string &prefix)
{
    std::string output = "block\n";
    if (statements.size() > 0)
    {
        for (int i = 0; i < statements.size() - 1; i++)
            output += prefix + "├──" + (statements.at(i) == nullptr ? "nullptr\n" : statements.at(i)->ToString(prefix + "│  "));
        output += prefix + "└──" + (statements.back() == nullptr ? "nullptr\n" : statements.back()->ToString(prefix + "   "));
    }
    return output;
}
