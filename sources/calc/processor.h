#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <deque>
#include <vector>

struct RegValue
{
    bool hasValue = false;
    double value = 0.0;
};

class Processor
{
public:
    Processor();
    
    void Push(double val);
    void Pop();
    void Replace(double x);
    void RollStackUp();
    void RollStackDown();
    void ShiftStackUp();
    
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
    
    void SetRegister(int regIdx, double value);
    bool GetRegister(int regIdx, double& value) const;
    void ClearRegister(int regIdx);
    size_t GetRegistersNumber() const
    {
        return m_registersNum;
    }
    
    auto& GetRegisters() const
    {
        return m_registers;
    }
    
private:
    void ShiftStackDown();
    
private:
    double m_prevX = 0;
    std::vector<double> m_stack;
    std::vector<RegValue> m_registers;
    const int m_maxStackSize = 4;
    const size_t m_registersNum = 32;
};



#endif // PROCESSOR_H
