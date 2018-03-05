#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include "command_base.h"

#include <iostream>

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

    void Print() const override
    {
        std::cout << m_number;
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

    void Print() const override
    {
        switch (m_operator)
        {
        case MathOperator::Plus:
            std::cout << "+";
            break;
        case MathOperator::Minus:
            std::cout << "-";
            break;
        case MathOperator::Mul:
            std::cout << "*";
            break;
        case MathOperator::Div:
            std::cout << "/";
            break;
        case MathOperator::Neg:
            std::cout << "neg";
            break;

        }
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

    void Print() const override
    {
        switch (m_function)
        {
        case MathFunction::Sin:
            std::cout << "sin";
            break;
        case MathFunction::Cos:
            std::cout << "cos";
            break;
        case MathFunction::Tan:
            std::cout << "tg";
            break;
        case MathFunction::Asin:
            std::cout << "arcsin";
            break;
        case MathFunction::Acos:
            std::cout << "arccos";
            break;
        case MathFunction::Atan:
            std::cout << "arctg";
            break;
        case MathFunction::Exp:
            std::cout << "exp";
            break;
        case MathFunction::Exp10:
            std::cout << "10x";
            break;
        case MathFunction::Exp2:
            std::cout << "2x";
            break;
        case MathFunction::Ln:
            std::cout << "ln";
            break;
        case MathFunction::Lg:
            std::cout << "lg";
            break;
        case MathFunction::Log:
            std::cout << "log";
            break;
        case MathFunction::Log2:
            std::cout << "log2";
            break;
        case MathFunction::Pow:
            std::cout << "pow";
            break;
        case MathFunction::Sqr:
            std::cout << "x2";
            break;
        case MathFunction::Sqrt:
            std::cout << "sqrt";
            break;
        case MathFunction::OneByX:
            std::cout << "1/x";
            break;
        case MathFunction::Abs:
            std::cout << "abs";
            break;
        case MathFunction::Frac:
            std::cout << "frac";
            break;
        case MathFunction::Int:
            std::cout << "int";
            break;
        case MathFunction::Sign:
            std::cout << "sign";
            break;
        case MathFunction::Max:
            std::cout << "max";
            break;
        case MathFunction::Min:
            std::cout << "min";
            break;
        case MathFunction::Clear:
            std::cout << "clear";
            break;
        }
    }
private:
    Function m_function;
};

#endif // MATH_OPERATIONS_H
