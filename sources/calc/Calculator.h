#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "math_operations.h"
#include "memory_operations.h"
#include "command_base.h"
#include "commands_visitor_base.h"
#include "processor.h"

using Commands = CommandsList<LoadNumberCommand, MathOperator, MathFunction, StackOperation, MemoryOperation>;

class Calculator : public CommandVisitor<Calculator, Commands>
{
public:
    Calculator();
    
    void Visit(const LoadNumberCommand& cmd, Processor* proc) const;
    void Visit(const MathOperator& oper, Processor* proc) const;
    void Visit(const MathFunction& oper, Processor* proc) const;
    void Visit(const StackOperation& oper, Processor* proc) const;
    void Visit(const MemoryOperation& oper, Processor* proc) const;
};

#endif // CALCULATOR_H
