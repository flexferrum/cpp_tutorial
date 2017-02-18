#ifndef MEMORY_OPERATIONS_H
#define MEMORY_OPERATIONS_H

#include "command_base.h"

class StackOperation : public CommandBase
{
public:
    enum Operation
    {
        SwapXy,
        RollUp,
        RollDown,
        ShiftUp,
        LoadPrevResult
    };
    
    StackOperation(Operation oper)
        : m_operation(oper)
    {
        
    }
    
    auto GetOperation() const
    {
        return m_operation;
    }
    
private:
    Operation m_operation;
};

class MemoryOperation : public CommandBase
{
public:
    enum Operation
    {
        LoadFromReg,
        StoreXToReg,
        StoreNumToReg,
        ClearReg
    };
    
    MemoryOperation(Operation oper, int regIdx, double num)
        : m_operation(oper)
        , m_register(regIdx)
        , m_number(num)
    {
        
    }
    
    auto GetOperation() const
    {
        return m_operation;
    }

    auto GetRegister() const
    {
        return m_register;
    }
    
    auto GetNumber() const
    {
        return m_number;
    }
    
private:
    Operation m_operation;
    int m_register;
    double m_number;
};

#endif // MEMORY_OPERATIONS_H
