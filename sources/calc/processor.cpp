#include "processor.h"

void Processor::Push(double val, bool updateLastRes)
{
    if (updateLastRes)
        m_lastResult = val;
    
    m_stack.push_front(val);
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
    if (!m_stack.empty())
        m_stack.pop_front();
}

bool Processor::TopPair(double &x, double &y) const
{
    if (m_stack.size() < 2)
        return false;
    
    x = m_stack[0];
    y = m_stack[1];
    return true;
}

void Processor::PopPair()
{
    Pop();
    Pop();
}
