#include "equal.h"

#include "../function.h"

std::unique_ptr<VarType> ASTExpressionEqual::ReturnType(ASTFunction &func)
{
    return VarTypeSimple::BoolType.Copy(); // a || b is always a boolean.
}

bool ASTExpressionEqual::IsLValue(ASTFunction &func)
{
    return false; // || operator works on two R-Values to produce an R-Value.
}

void ASTExpressionEqual::MyOptznPass(std::unique_ptr<ASTExpression> &parentPtr, ASTFunction &func)
{
    if (a1)
        a1->MyOptznPass(a1, func);
    if (a2)
        a2->MyOptznPass(a2, func);
}

llvm::Value *ASTExpressionEqual::Compile(llvm::IRBuilder<> &builder, ASTFunction &func) // Hm, this isn't the most efficient approach. I can think of a much easier way...
{

    // Make sure to cast both sides as booleans first.
    ASTExpression::ImplicitCast(func, a1, &VarTypeSimple::BoolType);
    ASTExpression::ImplicitCast(func, a2, &VarTypeSimple::BoolType);

    llvm::Value *res = builder.CreateICmpEQ(a1->CompileRValue(builder, func), a2->CompileRValue(builder, func), "result");
    return res;
}

std::string ASTExpressionEqual::ToString(const std::string &prefix)
{
    std::string ret = "(||)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}