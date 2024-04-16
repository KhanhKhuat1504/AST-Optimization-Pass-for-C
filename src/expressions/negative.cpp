#include "negative.h"

std::unique_ptr<VarType> ASTExpressionNegation::ReturnType(ASTFunction &func)
{
    return operand->ReturnType(func)->Copy();
}

bool ASTExpressionNegation::IsLValue(ASTFunction &func)
{
    return false;
}

llvm::Value *ASTExpressionNegation::Compile(llvm::IRBuilder<> &builder, ASTFunction &func)
{
    auto *value = operand->CompileRValue(builder, func);
    // Compile the values as needed. Remember, we can only do operations on R-Values.
    auto retType = ReturnType(func);
    if (retType->Equals(&VarTypeSimple::IntType)) // Do standard negation on integer operands since we return an int.
        return builder.CreateNeg(value);
    else if (retType->Equals(&VarTypeSimple::FloatType)) // Do negation on floating point operands since we return a float.
        return builder.CreateFNeg(value);
    else // Call to return type should make this impossible, but best to keep it here just in case of a bug.
        throw std::runtime_error("ERROR: Can not perform negation! Are both inputs either ints or floats?");
}

std::string ASTExpressionNegation::ToString(const std::string &prefix)
{
    std::string ret = "negate\n";
    ret += prefix + "├──" + operand->ToString(prefix + "│  ");
    return ret;
}