#include "for.h"

#include "../function.h"

std::unique_ptr<VarType> ASTStatementFor::StatementReturnType(ASTFunction &func)
{

    // It is completely possible for a for's condition to never be true, so even if does return something it's not confirmed.
    return nullptr;
}

void ASTStatementFor::Compile(llvm::Module &mod, llvm::IRBuilder<> &builder, ASTFunction &func)
{

    // ASTExpression::ImplicitCast(func, condition, &VarTypeSimple::BoolType);
    auto *funcVal = (llvm::Function *)func.GetVariableValue(func.name);
    auto forInitialization = llvm::BasicBlock::Create(builder.getContext(), "forInitialization", funcVal);
    auto forLoop = llvm::BasicBlock::Create(builder.getContext(), "forLoop", funcVal);
    auto forLoopBody = llvm::BasicBlock::Create(builder.getContext(), "forLoopBody", funcVal);
    auto forLoopEnd = llvm::BasicBlock::Create(builder.getContext(), "forLoopEnd", funcVal);

    builder.CreateBr(forInitialization);
    builder.SetInsertPoint(forInitialization);
    initialization->Compile(mod, builder, func);
    builder.CreateBr(forLoop);

    builder.SetInsertPoint(forLoop);
    auto conditionVal = condition->CompileRValue(builder, func);
    builder.CreateCondBr(conditionVal, forLoopBody, forLoopEnd);

    builder.SetInsertPoint(forLoopBody);
    thenStatement->Compile(mod, builder, func);
    increment->Compile(mod, builder, func);
    if (!thenStatement->StatementReturnType(func))
    {
        builder.CreateBr(forLoop);
    }
    builder.SetInsertPoint(forLoopEnd);
    // Create the basic blocks.
    // auto *funcVal = (llvm::Function *)func.GetVariableValue(func.name);
    // auto *forLoop = llvm::BasicBlock::Create(builder.getContext(), "forLoop", funcVal);
    // auto *forLoopBody = llvm::BasicBlock::Create(builder.getContext(), "forLoopBody", funcVal);
    // auto *forLoopEnd = llvm::BasicBlock::Create(builder.getContext(), "forLoopEnd", funcVal);

    // initialization->Compile(builder, func);
    // // Jump to the for loop.
    // builder.CreateBr(forLoop);

    // // Compile condition and jump to the right block.
    // builder.SetInsertPoint(forLoop);
    // auto *conditionVal = condition->CompileRValue(builder, func);
    // builder.CreateCondBr(conditionVal, forLoopBody, forLoopEnd);

    // // Compile the body. Note that we need to not create a jump if there is a return.
    // builder.SetInsertPoint(forLoopBody);
    // thenStatement->Compile(mod, builder, func);
    // increment->Compile(builder, func);
    // builder.CreateBr(forLoop);
    // // Continue from the end of the created for loop.
    // builder.SetInsertPoint(forLoopEnd);
}

std::string ASTStatementFor::ToString(const std::string &prefix)
{
    std::string output = "for\n";
    // output += prefix + "├──" + initialization->ToString(prefix + "│  ");
    output += prefix + "├──" + condition->ToString(prefix + "  ");
    // output += prefix + "└──" + increment->ToString(prefix + "   ");
    output += prefix + "└──" + thenStatement->ToString(prefix + "   ");
    return output;
}