#include "for.h"

#include "../function.h"

std::unique_ptr<VarType> ASTStatementFor::StatementReturnType(ASTFunction &func)
{

    // It is completely possible for a while's condition to never be true, so even if does return something it's not confirmed.
    return nullptr;
}

void ASTStatementFor::MyOptznPass(std::unique_ptr<ASTStatement> &parentPtr, ASTFunction &func)
{
    if (bodyStatement)
        bodyStatement->MyOptznPass(bodyStatement, func);
    if (initStatement)
        initStatement->MyOptznPass(initStatement, func);
    if (condition)
        condition->MyOptznPass(condition, func);
    if (incrementStatement)
        incrementStatement->MyOptznPass(incrementStatement, func);
}

void ASTStatementFor::Compile(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func)
{

    /*

        A while loop can be desugared to basic blocks. Take the following example:

            while (condition)
            {
                doThing();
            }

        This is really just another way of saying:

            whileLoop:
                if (condition) goto whileLoopBody else goto whileLoopEnd;

            whileLoopBody:
                doThing();
                goto whileLoop;

            whileLoopEnd:
                ...

    */

    // Create the basic blocks.
    auto *funcVal = (llvm::Function *)func.GetVariableValue(func.name);
    auto forLoop = llvm::BasicBlock::Create(builder.getContext(), "forLoop", funcVal);
    auto forLoopBody = llvm::BasicBlock::Create(builder.getContext(), "forLoopBody", funcVal);
    auto forLoopEnd = llvm::BasicBlock::Create(builder.getContext(), "forLoopEnd", funcVal);

    initStatement->Compile(mod, builder, func);
    // Jump to the while loop.
    builder.CreateBr(forLoop);

    // Compile condition and jump to the right block.
    builder.SetInsertPoint(forLoop);
    auto conditionVal = condition->CompileRValue(builder, func);
    builder.CreateCondBr(conditionVal, forLoopBody, forLoopEnd);

    // Compile the body. Note that we need to not create a jump if there is a return.
    builder.SetInsertPoint(forLoopBody);
    bodyStatement->Compile(mod, builder, func);
    incrementStatement->Compile(mod, builder, func);
    if (!bodyStatement->StatementReturnType(func))
        builder.CreateBr(forLoop);

    // Continue from the end of the created while loop.
    builder.SetInsertPoint(forLoopEnd);
}

std::string ASTStatementFor::ToString(const std::string &prefix)
{
    std::string output = "for\n";
    output += prefix + "├──" + initStatement->ToString(prefix + "│  ");
    output += prefix + "├──" + condition->ToString(prefix + "│  ");
    output += prefix + "├──" + incrementStatement->ToString(prefix + "│  ");
    output += prefix + "└──" + bodyStatement->ToString(prefix + "   ");
    return output;
}