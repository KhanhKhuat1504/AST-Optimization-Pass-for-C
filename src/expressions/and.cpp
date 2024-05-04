#include "and.h"

#include "../function.h"

std::unique_ptr<VarType> ASTExpressionAnd::ReturnType(ASTFunction &func)
{
    return VarTypeSimple::BoolType.Copy(); // a || b is always a boolean.
}

bool ASTExpressionAnd::IsLValue(ASTFunction &func)
{
    return false; // || operator works on two R-Values to produce an R-Value.
}

void ASTExpressionAnd::MyOptznPass(std::unique_ptr<ASTExpression> &parentPtr, ASTFunction &func)
{
    bool a1Optimzable = a1 && a1->IsConstant();
    bool a2Optimzable = a2 && a2->IsConstant();

    if (a1Optimzable)
        a1->MyOptznPass(a1, func);
    if (a2Optimzable)
        a2->MyOptznPass(a2, func);

    if (a1Optimzable)
    {
        if (a1->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
        {
            if (dynamic_cast<ASTExpressionBool *>(a1.get())->GetVal())
            {
                parentPtr.reset(a2.release());
                return;
            }
            else
            {
                parentPtr.reset(new ASTExpressionBool(false));
                return;
            }
        }
    }

    if (a2Optimzable)
    {
        if (a2->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
        {
            if (dynamic_cast<ASTExpressionBool *>(a2.get())->GetVal())
            {
                parentPtr.reset(a1.release());
                return;
            }
            else
            {
                parentPtr.reset(new ASTExpressionBool(false));
                return;
            }
        }
    }
}

llvm::Value *ASTExpressionAnd::Compile(llvm::IRBuilder<> &builder, ASTFunction &func) // Hm, this isn't the most efficient approach. I can think of a much easier way...
{
    // Make sure to cast both sides as booleans first.
    ASTExpression::ImplicitCast(func, a1, &VarTypeSimple::BoolType);
    ASTExpression::ImplicitCast(func, a2, &VarTypeSimple::BoolType);

    // Create blocks. Check right is if left is false, and we need to check the right one too. Continue block happens if true.
    auto *funcVal = (llvm::Function *)func.GetVariableValue(func.name);
    llvm::BasicBlock *checkRight = llvm::BasicBlock::Create(builder.getContext(), "checkRight", funcVal);
    llvm::BasicBlock *cont = llvm::BasicBlock::Create(builder.getContext(), "cont", funcVal);

    // If left is true, then we are done. Just branch to the continue block where we know the final result will be set as true.
    llvm::Value *leftVal = a1->CompileRValue(builder, func);
    llvm::BasicBlock *lastBlockLeft = builder.GetInsertBlock(); // Get the current block we are on.
    builder.CreateCondBr(leftVal, checkRight, cont);            // If return value is true so far we branch to checking the right one, else continue.

    // Compile the right expression if needed.
    builder.SetInsertPoint(checkRight);
    llvm::Value *rightVal = a2->CompileRValue(builder, func);
    llvm::BasicBlock *lastBlockRight = builder.GetInsertBlock(); // In case the block has changed, fix it.
    builder.CreateBr(cont);

    // Tell LLVM that it should either select the left value or the right one depending on where we came from.
    builder.SetInsertPoint(cont);
    llvm::PHINode *res = builder.CreatePHI(VarTypeSimple::BoolType.GetLLVMType(builder.getContext()), 2);
    res->addIncoming(leftVal, lastBlockLeft);
    res->addIncoming(rightVal, lastBlockRight);
    return res;
}

std::string ASTExpressionAnd::ToString(const std::string &prefix)
{
    std::string ret = "(&&)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}