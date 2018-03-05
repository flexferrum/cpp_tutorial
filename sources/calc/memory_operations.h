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

    void Print() const override
    {
        switch (m_operation)
        {
        case StackOperation::SwapXy:
            std::cout << "swap";
            break;
        case StackOperation::RollUp:
            std::cout << "rollup";
            break;
        case StackOperation::RollDown:
            std::cout << "rolldown";
            break;
        case StackOperation::ShiftUp:
            std::cout << "shift";
            break;
        case StackOperation::LoadPrevResult:
            std::cout << "lastx";
            break;

        }
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

    void Print() const override
    {
        switch (m_operation)
        {
        case MemoryOperation::LoadFromReg:
            std::cout << "loadreg r" << m_register;
            break;
        case MemoryOperation::StoreXToReg:
            std::cout << "storereg r" << m_register;
            break;
        case MemoryOperation::StoreNumToReg:
            std::cout << "storereg r" << m_register << ", " << m_number;
            break;
        case MemoryOperation::ClearReg:
            std::cout << "clearreg r" << m_register;
            break;
        }
    }

private:
    Operation m_operation;
    int m_register;
    double m_number;
};

#endif // MEMORY_OPERATIONS_H
