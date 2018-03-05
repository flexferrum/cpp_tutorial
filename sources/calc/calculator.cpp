#include "Calculator.h"

#include <iostream>
#include <functional>
#include <cmath>

struct NoCheck
{
    constexpr bool operator()(double /*x*/) const {return true;}
    constexpr bool operator()(double /*x*/, double /*y*/) const {return true;}
};

template<typename Fn = double (double), typename Checker = NoCheck>
void DoUnaryOper(Processor &calc, Fn oper, Checker checker = Checker())
{
    double x = 0;
    if (!calc.Top(x))
        return;

    if (!checker(x))
    {
        std::cout << "ERROR!" << std::endl;
        return;
    }

    calc.SetPrevX(x);
    auto val = oper(x);
    calc.Replace(val);
    std::cout << val << std::endl;
}

template<typename Fn = double (double, double), typename Checker = NoCheck>
void DoBinaryOper(Processor &calc, Fn oper, Checker checker = Checker())
{
    double x = 0;
    double y = 0;
    if (!calc.TopPair(x, y))
        return;

    if (!checker(x, y))
    {
        std::cout << "ERROR!" << std::endl;
        return;
    }

    calc.SetPrevX(x);
    auto val = oper(x, y);
    calc.Pop();
    calc.Replace(val);
    std::cout << val << std::endl;
}

Calculator::Calculator()
{

}

void Calculator::Visit(const LoadNumberCommand &cmd, Processor *proc) const
{
    proc->Push(cmd.GetNumber());
}

void Calculator::Visit(const MathOperator &oper, Processor *proc) const
{
    switch (oper.GetOperator())
    {
    case MathOperator::Plus:
        DoBinaryOper(*proc, std::plus<double>());
        break;
    case MathOperator::Minus:
        DoBinaryOper(*proc, std::minus<double>());
        break;
    case MathOperator::Div:
        DoBinaryOper(*proc, [](double x, double y) {return y / x;}, [](double x, double /*y*/) {return x != 0;});
        break;
    case MathOperator::Mul:
        DoBinaryOper(*proc, std::multiplies<double>());
        break;
    default:
        std::cout << "ASSERT!!! Unknown operation " << oper.GetOperator() << std::endl;
    }
}

void Calculator::Visit(const MathFunction &oper, Processor *proc) const
{
    switch (oper.GetFunction())
    {
    case MathFunction::Sin:
        DoUnaryOper(*proc, sin);
        break;
    case MathFunction::Cos:
        DoUnaryOper(*proc, cos);
        break;
    case MathFunction::Tan:
        DoUnaryOper(*proc, tan);
        break;
    case MathFunction::Asin:
        DoUnaryOper(*proc, asin);
        break;
    case MathFunction::Acos:
        DoUnaryOper(*proc, acos);
        break;
    case MathFunction::Atan:
        DoUnaryOper(*proc, atan);
        break;
    case MathFunction::Exp:
        DoUnaryOper(*proc, exp);
        break;
    case MathFunction::Exp10:
        DoUnaryOper(*proc, [](double x) {return pow(10, x);});
        break;
    case MathFunction::Exp2:
        DoUnaryOper(*proc, exp2);
        break;
    case MathFunction::Ln:
        DoUnaryOper(*proc, log);
        break;
    case MathFunction::Lg:
        DoUnaryOper(*proc, log10);
        break;
    case MathFunction::Log:
        DoBinaryOper(*proc, [](double x, double y) {return log(y) / log(x);});
        break;
    case MathFunction::Log2:
        DoUnaryOper(*proc, log2);
        break;
    case MathFunction::Pow:
        DoBinaryOper(*proc, [](double x, double y) {return pow(y, x);});
        break;
    case MathFunction::Sqr:
        DoUnaryOper(*proc, [](double x) {return x * x;});
        break;
    case MathFunction::Sqrt:
        DoUnaryOper(*proc, sqrt);
        break;
    case MathFunction::OneByX:
        DoUnaryOper(*proc, [](double x) {return 1 / x;}, [](double x) {return x != 0;});
        break;
    case MathFunction::Abs:
        DoUnaryOper(*proc, fabs);
        break;
    case MathFunction::Frac:
        DoUnaryOper(*proc, [](double x) {double tmp; return modf(x, &tmp);});
        break;
    case MathFunction::Int:
        DoUnaryOper(*proc, trunc);
        break;
    case MathFunction::Sign:
        DoUnaryOper(*proc, [](double x) {return signbit(x) ? -1 : 1;});
        break;
    case MathFunction::Max:
        DoBinaryOper(*proc, [](double x, double y) {return isgreater(y, x) ? y : x;});
        break;
    case MathFunction::Min:
        DoBinaryOper(*proc, [](double x, double y) {return isgreater(x, y) ? y : x;});
        break;
    case MathFunction::Clear:
        proc->Pop();
        break;
    default:
        std::cout << "ASSERT!!! Unknown operation " << oper.GetFunction() << std::endl;
    }
}

void Calculator::Visit(const StackOperation &oper, Processor *proc) const
{
    switch (oper.GetOperation())
    {
    case StackOperation::SwapXy:
    {
        double x;
        double y;

        proc->TopPair(x, y);
        proc->Pop();
        proc->Replace(x);
        proc->Push(y);
        break;
    }
    case StackOperation::RollUp:
        proc->RollStackUp();
        break;
    case StackOperation::RollDown:
        proc->RollStackDown();
        break;
    case StackOperation::ShiftUp:
        proc->ShiftStackUp();
        break;
    case StackOperation::LoadPrevResult:
    {
        double x = proc->GetPrevX();
        proc->Push(x);
        break;
    }
    default:
        std::cout << "ASSERT!!! Unknown operation " << oper.GetOperation() << std::endl;
    }
}

void Calculator::Visit(const MemoryOperation &oper, Processor *proc) const
{
    switch (oper.GetOperation())
    {
    case MemoryOperation::StoreXToReg:
    {
        double x = 0;
        if (proc->Top(x))
        {
            proc->SetRegister<RegType::Real>(oper.GetRegister(), x);
        }
        break;
    }
    case MemoryOperation::StoreNumToReg:
    {
        proc->SetRegister<RegType::Real>(oper.GetRegister(), oper.GetNumber());
        break;
    }
    case MemoryOperation::LoadFromReg:
    {
        auto& valReg = proc->GetRegister<RegType::Real>(oper.GetRegister());
        if (valReg.hasValue)
            proc->Push(valReg.value);
        break;
    }
    case MemoryOperation::ClearReg:
        proc->ClearRegister<RegType::Real>(oper.GetRegister());
        break;
    default:
        std::cout << "ASSERT!!! Unknown operation " << oper.GetOperation() << std::endl;
    }
}
