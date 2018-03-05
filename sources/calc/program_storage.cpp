#include "program_storage.h"

class ProgramExecutor : public CommandVisitor<ProgramExecutor, Commands>
{
public:
    ProgramExecutor(Calculator *calc)
        : m_calculator(calc)
    {
    }

    void Visit(const CommandBase& cmd, Processor* proc) const
    {
        m_calculator->VisitCommand(&cmd, proc);
        proc->SetEP(proc->GetEP() + 1);
    }

private:
    Calculator* m_calculator;
};

ProgramStorage::ProgramStorage()
{

}

void ProgramStorage::Visit(const LoadNumberCommand& cmd, Processor*) const
{
    const_cast<ProgramStorage*>(this)->AddInstruction(std::make_shared<LoadNumberCommand>(cmd));
}

void ProgramStorage::Visit(const MathOperator& oper, Processor*) const
{
    const_cast<ProgramStorage*>(this)->AddInstruction(std::make_shared<MathOperator>(oper));
}

void ProgramStorage::Visit(const MathFunction& oper, Processor*) const
{
    const_cast<ProgramStorage*>(this)->AddInstruction(std::make_shared<MathFunction>(oper));
}

void ProgramStorage::Visit(const StackOperation& oper, Processor*) const
{
    const_cast<ProgramStorage*>(this)->AddInstruction(std::make_shared<StackOperation>(oper));
}

void ProgramStorage::Visit(const MemoryOperation& oper, Processor*) const
{
    const_cast<ProgramStorage*>(this)->AddInstruction(std::make_shared<MemoryOperation>(oper));
}

void ProgramStorage::SetNextInstruction(size_t idx)
{

}

size_t ProgramStorage::GetNextInstruction() const
{
    return 0xffffffffffffffff;
}

void ProgramStorage::Execute(Calculator* calc, Processor* proc)
{
    proc->SetEP(0);

    while (proc->GetEP() < m_instructions.size())
        ExecuteNext(calc, proc);
}

void ProgramStorage::ExecuteNext(Calculator* calc, Processor* proc)
{
    auto instr = m_instructions[proc->GetEP()];
    ProgramExecutor executor(calc);
    executor.VisitCommand(instr.get(), proc);
}

void ProgramStorage::PrintAll()
{
    std::cout << "Program start" << std::endl;
    int idx = 0;
    for (auto& inst : m_instructions)
    {
        printf("%03d\t", idx);
        PrintInstruction(inst.get());
        idx ++;
    }
    std::cout << "Program end" << std::endl;
}

void ProgramStorage::Print(size_t idx)
{

}

void ProgramStorage::AddInstruction(std::shared_ptr<CommandBase> instruction)
{
    m_instructions.push_back(instruction);
}

void ProgramStorage::PrintInstruction(const CommandBase* inst)
{
    inst->Print();
    std::cout << std::endl;
}
