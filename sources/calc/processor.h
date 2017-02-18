#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <deque>
#include <vector>

class Processor
{
public:
    Processor();
    
    void Push(double val);
    void Pop();
    void Replace(double x);
    void RollStackUp();
    void RollStackDown();
    
    bool Top(double &x) const;    
    bool TopPair(double &x, double &y) const;
    
    void SetPrevX(double x)
    {
        m_prevX = x;
    }
    auto GetPrevX() const
    {
        return m_prevX;
    }
    
    const auto &GetStack() const
    {
        return m_stack;
    }
    
private:
    void ShiftStackUp();
    void ShiftStackDown();
    
private:
    double m_prevX = 0;
    std::vector<double> m_stack;
    std::vector<double> m_registers;
    const int m_maxStackSize = 4;
};



#endif // PROCESSOR_H
