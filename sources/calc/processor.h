#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <deque>
#include <vector>

class Processor
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



#endif // PROCESSOR_H
