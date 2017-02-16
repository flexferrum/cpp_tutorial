#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <deque>
#include <vector>

class Calculator
{
public:
    void Push(double val, bool updateLastRes);
    
    bool Top(double &x) const;
    void Pop();
    
    bool TopPair(double &x, double &y) const;
    void PopPair();
    
    const auto &GetStack() const
    {
        return m_stack;
    }
    
    auto GetLastResult() const
    {
        return m_lastResult;
    }
    
private:
    double m_lastResult = 0;
    std::deque<double> m_stack;
    std::vector<double> m_registers;
    // const int m_maxStackSize = 4;
};

struct NoCheck
{
    constexpr bool operator()(double /*x*/) const {return true;}
    constexpr bool operator()(double /*x*/, double /*y*/) const {return true;}
};

template<typename Fn = double (double), typename Checker = NoCheck>
void DoUnaryOper(Calculator &calc, Fn oper, Checker checker = Checker())
{
    double x = 0;
    if (!calc.Top(x))
        return;
    
    if (!checker(x))
    {
        std::cout << "ERROR!" << std::endl;
        return;
    }
    
    auto val = oper(x);
    calc.Pop();
    calc.Push(val, true);
    std::cout << val << std::endl;    
}

template<typename Fn = double (double, double), typename Checker = NoCheck>
void DoBinaryOper(Calculator &calc, Fn oper, Checker checker = Checker())
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
    
    auto val = oper(x, y);
    calc.PopPair();
    calc.Push(val, true);
    std::cout << val << std::endl;    
}


#endif // CALCULATOR_H
