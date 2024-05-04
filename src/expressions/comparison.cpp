#include "comparison.h"

std::unique_ptr<VarType> ASTExpressionComparison::ReturnType(ASTFunction &func)
{
    return VarTypeSimple::BoolType.Copy();
}

bool ASTExpressionComparison::IsLValue(ASTFunction &func)
{
    return false; // If we are adding values together, they must be usable R-Values. Adding these together just results in an R-Value.
}

void ASTExpressionComparison::MyOptznPass(std::unique_ptr<ASTExpression> &parentPtr, ASTFunction &func)
{
    std::cout << "Optimizing Comparison." << std::endl;
    bool a1Optimizable = false;
    bool a2Optimizable = false;
    float a1Val = 0;
    float a2Val = 0;

    if (a1)
    {
        if (a1->IsConstant())
        {
            std::cout << "A1 Constant." << std::endl;
            if (a1->ReturnType(func)->Equals(&VarTypeSimple::IntType))
            {
                a1Optimizable = true;
                a1Val = dynamic_cast<ASTExpressionInt *>(a1.get())->GetVal();
            }
            else if (a1->ReturnType(func)->Equals(&VarTypeSimple::FloatType))
            {
                a1Optimizable = true;
                a1Val = dynamic_cast<ASTExpressionFloat *>(a1.get())->GetVal();
            }
            else if (a1->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
            {
                a1Optimizable = true;
                if (dynamic_cast<ASTExpressionBool *>(a1.get())->GetVal())
                {
                    a1Val = 1.0f;
                }
                else
                {
                    a1Val = 0.0f;
                }
            }
        }
        else
        {
            std::cout << "A1 NOT Constant." << std::endl;
            a1->MyOptznPass(a1, func);
        }
    }

    if (a2)
    {
        if (a2->IsConstant())
        {
            if (a2->ReturnType(func)->Equals(&VarTypeSimple::IntType))
            {
                a2Optimizable = true;
                a2Val = dynamic_cast<ASTExpressionInt *>(a2.get())->GetVal();
            }
            else if (a2->ReturnType(func)->Equals(&VarTypeSimple::FloatType))
            {
                a2Optimizable = true;
                a2Val = dynamic_cast<ASTExpressionFloat *>(a2.get())->GetVal();
            }
            else if (a2->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
            {
                a2Optimizable = true;
                if (dynamic_cast<ASTExpressionBool *>(a1.get())->GetVal())
                {
                    a1Val = 1.0f;
                }
                else
                {
                    a1Val = 0.0f;
                }
            }
        }
        else
        {
            a2->MyOptznPass(a2, func);
        }
    }

    if (a1Optimizable && a2Optimizable)
    {
        switch (type)
        {
        case Equal:
            parentPtr.reset(new ASTExpressionBool(a1Val == a2Val));
            break;
        case NotEqual:
            parentPtr.reset(new ASTExpressionBool(a1Val != a2Val));
            break;
        case LessThan:
            parentPtr.reset(new ASTExpressionBool(a1Val < a2Val));
            break;
        case LessThanOrEqual:
            parentPtr.reset(new ASTExpressionBool(a1Val <= a2Val));
            break;
        case GreaterThan:
            parentPtr.reset(new ASTExpressionBool(a1Val > a2Val));
            break;
        case GreaterThanOrEqual:
            parentPtr.reset(new ASTExpressionBool(a1Val >= a2Val));
            break;
        }
    }
}

llvm::Value *ASTExpressionComparison::Compile(llvm::IRBuilder<> &builder, ASTFunction &func)
{
    VarTypeSimple *returnType;
    if (!ASTExpression::CoerceTypes(func, a1, a2, returnType)) // This will force our arguments to be the same type and outputs which one it is.
        throw std::runtime_error("ERROR: Can not coerce types in comparison expression! Are they all booleans, ints, and floats?");

    // Get values. Operations only work on R-Values.
    auto a1Val = a1->CompileRValue(builder, func);
    auto a2Val = a2->CompileRValue(builder, func);

    // Int type, do int comparisons.
    if (returnType->Equals(&VarTypeSimple::IntType))
    {
        switch (type)
        {
        case Equal:
            return builder.CreateICmp(llvm::CmpInst::ICMP_EQ, a1Val, a2Val);
        case NotEqual:
            return builder.CreateICmp(llvm::CmpInst::ICMP_NE, a1Val, a2Val);
        case LessThan:
            return builder.CreateICmp(llvm::CmpInst::ICMP_SLT, a1Val, a2Val);
        case LessThanOrEqual:
            return builder.CreateICmp(llvm::CmpInst::ICMP_SLE, a1Val, a2Val);
        case GreaterThan:
            return builder.CreateICmp(llvm::CmpInst::ICMP_SGT, a1Val, a2Val);
        case GreaterThanOrEqual:
            return builder.CreateICmp(llvm::CmpInst::ICMP_SGE, a1Val, a2Val);
        }
    }

    // Float type, do float comparisons.
    else if (returnType->Equals(&VarTypeSimple::FloatType))
    {
        switch (type)
        {
        case Equal:
            return builder.CreateFCmp(llvm::CmpInst::ICMP_EQ, a1Val, a2Val);
        case NotEqual:
            return builder.CreateFCmp(llvm::CmpInst::ICMP_NE, a1Val, a2Val);
        case LessThan:
            return builder.CreateFCmp(llvm::CmpInst::ICMP_SLT, a1Val, a2Val);
        case LessThanOrEqual:
            return builder.CreateFCmp(llvm::CmpInst::ICMP_SLE, a1Val, a2Val);
        case GreaterThan:
            return builder.CreateFCmp(llvm::CmpInst::ICMP_SGT, a1Val, a2Val);
        case GreaterThanOrEqual:
            return builder.CreateFCmp(llvm::CmpInst::ICMP_SGE, a1Val, a2Val);
        }
    }

    else
    {
        throw std::runtime_error("ERROR: Did not return value from comparison. Unsuccessful coercion of values or invalid comparison type!");
    }
}

std::string ASTExpressionComparison::ToString(const std::string &prefix)
{
    std::string op = "";
    switch (type)
    {
    case Equal:
        op = "=";
        break;
    case NotEqual:
        op = "!=";
        break;
    case LessThan:
        op = "<";
        break;
    case LessThanOrEqual:
        op = "<=";
        break;
    case GreaterThan:
        op = ">";
        break;
    case GreaterThanOrEqual:
        op = ">=";
        break;
    }
    std::string ret = "(" + op + ")\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}