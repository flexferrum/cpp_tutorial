#include "processor.h"

#include <cassert>

Processor::Processor()
{
    m_stack.reserve(m_maxStackSize * 2);
    m_stack.resize(m_maxStackSize, 0.0);
    m_registers.resize(m_registersNum);
}

void Processor::ShiftStackUp()
{
    std::copy(m_stack.rbegin() + 1, m_stack.rend(), m_stack.rbegin());
}

void Processor::ShiftStackDown()
{
    std::copy(m_stack.begin() + 1, m_stack.end(), m_stack.begin());    
}

void Processor::Push(double val)
{
    ShiftStackUp();
    m_stack[0] = val;
}

void Processor::Replace(double x)
{
    m_stack[0] = x;
}

void Processor::RollStackUp()
{
    double val = m_stack[m_maxStackSize - 1];
    ShiftStackUp();
    m_stack[0] = val;
}

void Processor::RollStackDown()
{
    double val = m_stack[0];
    ShiftStackDown();
    m_stack[m_maxStackSize - 1] = val;
}

bool Processor::Top(double &x) const
{
    if (m_stack.empty())
        return false;
    
    x = m_stack[0];
    return true;
}

void Processor::Pop()
{
    ShiftStackDown();
}

bool Processor::TopPair(double &x, double &y) const
{
    if (m_stack.size() < 2)
        return false;
    
    x = m_stack[0];
    y = m_stack[1];
    return true;
}

void Processor::SetRegister(int regIdx, double value)
{
    assert(regIdx >= 0 && regIdx < m_registersNum);
    
    auto& reg = m_registers[regIdx];
    reg.hasValue = true;
    reg.value = value;
}

bool Processor::GetRegister(int regIdx, double &value) const
{
    assert(regIdx >= 0 && regIdx < m_registersNum);
    
    auto& reg = m_registers[regIdx];
    if (reg.hasValue)
        value = reg.value;
    
    return reg.hasValue;
}

void Processor::ClearRegister(int regIdx)
{
    assert(regIdx >= 0 && regIdx < m_registersNum);
    
    auto& reg = m_registers[regIdx];
    reg.hasValue = false;
    reg.value = 0;
}
