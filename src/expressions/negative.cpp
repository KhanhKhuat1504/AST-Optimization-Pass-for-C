#include "negative.h"

std::unique_ptr<VarType> ASTExpressionNegation::ReturnType(ASTFunction &func)
{
    if (!returnType) // If the return type has not been gotten yet.
    {
        if (a->ReturnType(func)->Equals(&VarTypeSimple::IntType))
            returnType = &VarTypeSimple::IntType;
        else if (a->ReturnType(func)->Equals(&VarTypeSimple::FloatType))
            returnType = &VarTypeSimple::FloatType;
        else
            throw std::runtime_error("ERROR: Can not coerce types in addition expression! Are they both either ints or floats?");
    }
    return std::make_unique<VarTypeSimple>(*returnType); // Make a copy of our return type :}
}

bool ASTExpressionNegation::IsLValue(ASTFunction &func)
{
    return false; // If we are adding values together, they must be usable R-Values. Adding these together just results in an R-Value.
}

void ASTExpressionNegation::MyOptznPass(std::unique_ptr<ASTExpression> &parentPtr, ASTFunction &func)
{
    if (a)
        a->MyOptznPass(a, func);
}

llvm::Value *ASTExpressionNegation::Compile(llvm::IRBuilder<> &builder, ASTFunction &func)
{
    // Compile the values as needed. Remember, we can only do operations on R-Values.
    auto retType = ReturnType(func);
    if (retType->Equals(&VarTypeSimple::IntType)) // Do standard addition on integer operands since we return an int.
        return builder.CreateNeg(a->CompileRValue(builder, func));
    else if (retType->Equals(&VarTypeSimple::FloatType)) // Do addition on floating point operands since we return a float.
        return builder.CreateFNeg(a->CompileRValue(builder, func));
    else // Call to return type should make this impossible, but best to keep it here just in case of a bug.
        throw std::runtime_error("ERROR: Can not perform addition! Are both inputs either ints or floats?");
}

std::string ASTExpressionNegation::ToString(const std::string &prefix)
{
    std::string ret = "(-)\n";
    ret += prefix + "├──" + a->ToString(prefix + "│  ");
    return ret;
}