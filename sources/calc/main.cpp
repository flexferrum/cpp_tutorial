#include "processor.h"
#include "input_parser.h"
#include "command_base.h"
#include "commands_visitor_base.h"
#include "Calculator.h"
#include "math_operations.h"
#include "program_storage.h"

#include <iostream>
#include <regex>
#include <functional>
#include <map>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <cmath>

enum class CurrentOperationMode
{
    Calculate,
    Program,
    Debug
};

template<typename Key, typename Fn>
bool ProcessCommand(const std::map<Key, Fn> &commands, const Key &cmd, Processor &calc, const InputData& inputData)
{
   auto p = commands.find(cmd);
   if (p == commands.end())
       return false;

   p->second(calc, inputData);
   return true;
}

void ShowStack(Processor &calc, const InputData&)
{
    auto &stack = calc.GetStack();

    auto idx = stack.size();

    std::for_each(stack.rbegin(), stack.rend(), [&idx](double val)
    {
        if (idx == 2)
            std::cout << "2 (y): ";
        else if (idx == 1)
            std::cout << "1 (x): ";
        else
            std::cout << idx << "    : ";
        std::cout << val << std::endl;
        idx --;
    });


    std::cout << "x0   : " << calc.GetPrevX() << std::endl;
}

void ShowRegisters(Processor &proc, const InputData&)
{
    auto& realRegs = proc.GetRegisters<RegType::Real>();

    for (size_t n = 0; n < realRegs.size(); ++ n)
    {
        auto& reg = realRegs[n];
        std::cout << "r" << n << ": ";
        if (reg.hasValue)
            std::cout << reg.value;
        else
            std::cout << "<empty>";

        if (n % 2)
            std::cout << std::endl;
        else
            std::cout << '\t';
    }
}

void DumpInputData(const InputData& result)
{
    std::cout << "!!! result.number = " << result.number << std::endl;
    std::cout << "!!! result.isNumber = " << result.isNumber << std::endl;
    std::cout << "!!! result.commandType = " << result.commandType << std::endl;
    std::cout << "!!! result.command = \'" << result.command << "\'" << std::endl;
    std::cout << "!!! result.commandParam = \'" << result.commandParam << "\'" << std::endl;
    std::cout << "!!! result.parseError = " << result.parseError << std::endl;
}

template<typename Cmd, typename ... Args>
std::shared_ptr<CommandBase> MakeCommand(Args&& ... args)
{
    return std::make_shared<Cmd>(std::forward<Args>(args)...);
}

std::shared_ptr<CommandBase> MakeMemoryCommand(MemoryOperation::Operation operId, Processor &proc, const InputData& inputData)
{
    static std::regex parser("r([0-9]+)(?:,[[:space:]]*(.+))?");
    std::shared_ptr<CommandBase> result;

    auto matchBegin = std::sregex_iterator(inputData.commandParam.begin(), inputData.commandParam.end(), parser);
    auto matchEnd = std::sregex_iterator();

    auto matches = std::distance(matchBegin, matchEnd);
    if (matches == 0)
    {
        std::cout << "ERROR! Need correct register address!" << std::endl;
        return result;
    }
    auto match = *matchBegin;

    auto regName = match.str(1);

    double number = 0.0;
    if (match.size() == 3 && operId == MemoryOperation::StoreXToReg)
    {
        std::string strNum = match.str(2);
        if (InputParser::ParseNumber(strNum, number))
            operId = MemoryOperation::StoreNumToReg;
    }

    int regIdx = std::atoi(regName.c_str());
    if (regIdx < 0 || regIdx >= proc.GetRegistersNumber<RegType::Real>())
    {
        std::cout << "ERROR! Invalid register index. Should be in range [0 - " << proc.GetRegistersNumber<RegType::Real>() << "]" << std::endl;
        return result;
    }
    return std::make_shared<MemoryOperation>(operId, regIdx, number);
}

int main()
{
    Processor processor;
    Calculator calc;
    ProgramStorage programStorage;
    CurrentOperationMode operationMode;

    using Fn = std::function<void (Processor&, const InputData&)>;

    std::shared_ptr<CommandBase> command;
    bool doQuit = false;

    std::map<char, Fn> operators =
    {
        {'+', [&command](Processor &, const InputData&){command = MakeCommand<MathOperator>(MathOperator::Plus);}},
        {'-', [&command](Processor &, const InputData&){command = MakeCommand<MathOperator>(MathOperator::Minus);}},
        {'*', [&command](Processor &, const InputData&){command = MakeCommand<MathOperator>(MathOperator::Mul);}},
        {'/', [&command](Processor &, const InputData&){command = MakeCommand<MathOperator>(MathOperator::Div);}},
    };

    std::map<std::string, Fn> functions =
    {
        {"sin", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Sin);}},
        {"cos", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Cos);}},
        {"tg", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Tan);}},
        {"arcsin", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Asin);}},
        {"arccos", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Acos);}},
        {"arctg", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Atan);}},
        {"exp", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Exp);}},
        {"10x", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Exp10);}},
        {"2x", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Exp2);}},
        {"ln", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Ln);}},
        {"lg", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Lg);}},
        {"log2", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Log2);}},
        {"logyx", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Log);}},
        {"pow", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Pow);}},
        {"x2", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Sqr);}},
        {"sqrt", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Sqrt);}},
        {"1/x", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::OneByX);}},
        {"abs", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Abs);}},
        {"frac", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Frac);}},
        {"int", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Int);}},
        {"sign", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Sign);}},
        {"max", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Max);}},
        {"min", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Min);}},
        {"clear", [&command](Processor &, const InputData&){command = MakeCommand<MathFunction>(MathFunction::Clear);}},
        {"neg", [&command](Processor &, const InputData&){command = MakeCommand<MathOperator>(MathOperator::Neg);}},
        {"pi", [&command](Processor &, const InputData&){command = MakeCommand<LoadNumberCommand>(3.14159265358979323);}},
        {"swap", [&command](Processor &, const InputData&){command = MakeCommand<StackOperation>(StackOperation::SwapXy);}},
        {"rollup", [&command](Processor &, const InputData&){command = MakeCommand<StackOperation>(StackOperation::RollUp);}},
        {"rolldown", [&command](Processor &, const InputData&){command = MakeCommand<StackOperation>(StackOperation::RollDown);}},
        {"shift", [&command](Processor &, const InputData&){command = MakeCommand<StackOperation>(StackOperation::ShiftUp);}},
        {"lastx", [&command](Processor &, const InputData&){command = MakeCommand<StackOperation>(StackOperation::LoadPrevResult);}},
        {"storereg", [&command](Processor &proc, const InputData& inputData){command = MakeMemoryCommand(MemoryOperation::StoreXToReg, proc, inputData);}},
        {"loadreg", [&command](Processor &proc, const InputData& inputData){command = MakeMemoryCommand(MemoryOperation::LoadFromReg, proc, inputData);}},
        {"clearreg", [&command](Processor &proc, const InputData& inputData){command = MakeMemoryCommand(MemoryOperation::ClearReg, proc, inputData);}},
    };

    std::map<std::string, Fn> commands =
    {
        {"quit", [&doQuit](Processor &, const InputData&){doQuit = true;}},
        {"list", [&programStorage](Processor &, const InputData&){programStorage.PrintAll();}},
        {"stack", ShowStack},
        {"regs", ShowRegisters},
        {"run", [&calc, &programStorage](Processor &proc, const InputData&){programStorage.Execute(&calc, &proc);}},
        {"enterprg", [&operationMode](Processor &, const InputData&){operationMode = CurrentOperationMode::Program;}},
        {"exitprg", [&operationMode](Processor &, const InputData&){operationMode = CurrentOperationMode::Calculate;}},
    };

    InputParser inputParser;
    AppendRegExpString(inputParser, "", operators);
    AppendRegExpString(inputParser, "?:", functions);
    AppendRegExpString(inputParser, "?:#", commands);
    inputParser.PrepareRegExp();

    while (!doQuit)
    {
        if (command)
            command.reset();

        std::cout << "> ";
        std::cout.flush();
        std::string line;
        std::getline(std::cin, line);

        auto result = inputParser.ParseLine(line);

        if (!result.parseError.empty())
        {
            std::cout << "ERROR: " << result.parseError << std::endl;
            continue;
        }

        bool isProcessed = false;
        if (result.isNumber)
        {
            command = MakeCommand<LoadNumberCommand>(result.number);
        }
        else
        {
            switch (result.commandType)
            {
            case 0:
                isProcessed = ProcessCommand(operators, result.command[0], processor, result);
                break;
            case 1:
                isProcessed = ProcessCommand(functions, result.command, processor, result);
                break;
            case 2:
                isProcessed = ProcessCommand(commands, result.command, processor, result);
                break;
            default:
                break;
            }

            if (!isProcessed)
                std::cout << "ERROR: Unrecognised input string!" << std::endl;
        }

        if (command)
        {
            switch (operationMode)
            {
            case CurrentOperationMode::Calculate:
                calc.VisitCommand(command.get(), &processor);
                break;
            case CurrentOperationMode::Program:
                programStorage.VisitCommand(command.get(), &processor);
                break;
            default:
                break;
            }
        }
    }

    std::cout << "Well done, commander!" << std::endl;

    return 0;
}
