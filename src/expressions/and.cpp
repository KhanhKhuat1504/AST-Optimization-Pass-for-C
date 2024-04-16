#include "and.h"

#include "../function.h"

std::unique_ptr<VarType> ASTExpressionAnd::ReturnType(ASTFunction &func)
{
    return VarTypeSimple::BoolType.Copy(); // a && b is always a boolean.
}

bool ASTExpressionAnd::IsLValue(ASTFunction &func)
{
    return false;
}

llvm::Value *ASTExpressionAnd::Compile(llvm::IRBuilder<> &builder, ASTFunction &func)
{

    // Make sure to cast both sides as booleans first.
    ASTExpression::ImplicitCast(func, a1, &VarTypeSimple::BoolType);
    ASTExpression::ImplicitCast(func, a2, &VarTypeSimple::BoolType);

    // Create blocks.
    auto *funcVal = (llvm::Function *)func.GetVariableValue(func.name);
    auto *falseBlock = llvm::BasicBlock::Create(builder.getContext(), "falseBlock", funcVal);
    auto *trueBlock = llvm::BasicBlock::Create(builder.getContext(), "trueBlock", funcVal);
    auto *cont = llvm::BasicBlock::Create(builder.getContext(), "cont", funcVal);

    auto *leftVal = a1->CompileRValue(builder, func);
    builder.CreateCondBr(leftVal, trueBlock, falseBlock);

    // Compile the right expression
    builder.SetInsertPoint(trueBlock);
    auto *rightVal = a2->CompileRValue(builder, func);

    // After evaluating the right side, branch to cont.
    builder.CreateBr(cont);
    builder.SetInsertPoint(falseBlock);
    builder.CreateBr(cont);

    // Tell LLVM that it should either select the left value and the right one depending on where we came from.
    builder.SetInsertPoint(cont);
    llvm::PHINode *res = builder.CreatePHI(VarTypeSimple::BoolType.GetLLVMType(builder.getContext()), 2);
    res->addIncoming(rightVal, trueBlock);
    res->addIncoming(leftVal, falseBlock);
    return res;
}

std::string ASTExpressionAnd::ToString(const std::string &prefix)
{
    std::string ret = "(&&)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}