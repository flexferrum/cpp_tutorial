#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <deque>
#include <vector>
#include <cassert>

template<typename T>
struct RegValue
{
    bool hasValue = false;
    T value = 0;
};

using RegIntValue = RegValue<int>;
using RegRealValue = RegValue<double>;

enum class RegType
{
    Integer,
    Real
};

template<typename VT, RegType RT>
class RegistersBlock
{
public:
    using ValueType = RegValue<VT>;

    void Init(size_t regsCount)
    {
        m_regsCount = regsCount;
        m_registers.resize(regsCount);
    }

    auto GetValue(int regIdx) const
    {
        assert(regIdx >= 0 && regIdx < m_regsCount);

        return m_registers[regIdx];
    }

    void SetValue(int regIdx, const VT &value)
    {
        assert(regIdx >= 0 && regIdx < m_regsCount);

        auto& reg = m_registers[regIdx];
        reg.hasValue = true;
        reg.value = value;
    }

    void ClearValue(int regIdx)
    {
        assert(regIdx >= 0 && regIdx < m_regsCount);

        auto& reg = m_registers[regIdx];
        reg.hasValue = false;
        reg.value = 0;
    }

    size_t GetCount() const
    {
        return m_regsCount;
    }

    auto& GetAllValues() const
    {
        return m_registers;
    }

private:
    size_t m_regsCount = 0;
    std::vector<ValueType> m_registers;
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

    template<RegType RT, typename T>
    void SetRegister(int regIdx, T value)
    {
        RegValueAccessor<RT>::Get(this).SetValue(regIdx, value);
    }
    template<RegType RT>
    auto GetRegister(int regIdx) const
    {
        return RegValueAccessor<RT>::Get(this).GetValue(regIdx);
    }
    template<RegType RT>
    void ClearRegister(int regIdx)
    {
        RegValueAccessor<RT>::Get(this).ClearValue(regIdx);
    }
    template<RegType RT>
    size_t GetRegistersNumber() const
    {
        return RegValueAccessor<RT>::Get(this).GetCount();
    }
    template<RegType RT>
    auto& GetRegisters() const
    {
        return RegValueAccessor<RT>::Get(this).GetAllValues();
    }

    auto GetEP() const
    {
        return m_executionPoint;
    }
    void SetEP(size_t ep)
    {
        m_executionPoint = ep;
    }

private:
    template<RegType RT>
    struct RegValueAccessor;

    template<>
    struct RegValueAccessor<RegType::Integer>
    {
        static const auto& Get(const Processor* proc)
        {
            return proc->m_intRegisters;
        }
        static auto& Get(Processor* proc)
        {
            return proc->m_intRegisters;
        }
    };

    template<>
    struct RegValueAccessor<RegType::Real>
    {
        static const auto& Get(const Processor* proc)
        {
            return proc->m_realRegisters;
        }
        static auto& Get(Processor* proc)
        {
            return proc->m_realRegisters;
        }
    };

    void ShiftStackDown();

private:
    double m_prevX = 0;
    size_t m_executionPoint = 0;

    std::vector<double> m_stack;
    RegistersBlock<double, RegType::Real> m_realRegisters;
    RegistersBlock<int, RegType::Integer> m_intRegisters;
    const int m_maxStackSize = 4;
    const size_t m_realRegistersNum = 32;
    const size_t m_intRegistersNum = 8;
};



#endif // PROCESSOR_H
