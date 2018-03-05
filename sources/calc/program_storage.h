#ifndef PROGRAM_STORAGE_H
#define PROGRAM_STORAGE_H

#include "calculator.h"



class ProgramStorage : public CommandVisitor<ProgramStorage, Commands>
{
public:
    ProgramStorage();

    void Visit(const LoadNumberCommand& cmd, Processor* /*proc*/) const;
    void Visit(const MathOperator& oper, Processor* /*proc*/) const;
    void Visit(const MathFunction& oper, Processor* /*proc*/) const;
    void Visit(const StackOperation& oper, Processor* /*proc*/) const;
    void Visit(const MemoryOperation& oper, Processor* /*proc*/) const;

    void SetNextInstruction(size_t idx);
    size_t GetNextInstruction() const;

    void Execute(Calculator* calc, Processor* proc);
    void ExecuteNext(Calculator* calc, Processor* proc);
    void PrintAll();
    void Print(size_t idx);

private:

    using InstructionsList = std::vector<std::shared_ptr<CommandBase>>;
    InstructionsList m_instructions;

private:
    void AddInstruction(std::shared_ptr<CommandBase> instruction);
    void PrintInstruction(const CommandBase* inst);
};

#endif // PROGRAM_STORAGE_H
