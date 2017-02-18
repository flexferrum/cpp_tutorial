#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include "command_base.h"

class LoadNumberCommand : public CommandBase
{
public:
    LoadNumberCommand(double num)
        : m_number(num)
    {
        
    }
    
    auto GetNumber() const 
    {
        return m_number;
    }
private:
    double m_number;
};

class MathOperator : public CommandBase
{
public:
    enum Operator
    {
        Plus,
        Minus,
        Mul,
        Div,
        Neg
    };
    
    MathOperator(Operator oper)
        : m_operator(oper)
    {
        
    }
    
    auto GetOperator() const
    {
        return m_operator;
    }
    
private:
    Operator m_operator;
};

class MathFunction : public CommandBase
{
public:
    enum Function
    {
        Sin,
        Cos,
        Tan,
        Asin,
        Acos,
        Atan,
        Exp,
        Exp10,
        Exp2,
        Ln,
        Lg,
        Log,
        Log2,
        Pow,
        Sqr,
        Sqrt,
        OneByX,
        Abs,
        Frac,
        Int,
        Sign,
        Max,
        Min,
        Clear,
    };
    
    MathFunction(Function fn)
        : m_function(fn)
    {
        
    }
    
    auto GetFunction() const
    {
        return m_function;
    }
private:
    Function m_function;
};

#endif // MATH_OPERATIONS_H
