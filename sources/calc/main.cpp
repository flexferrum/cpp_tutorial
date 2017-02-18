#include "processor.h"
#include "input_parser.h"
#include "command_base.h"
#include "commands_visitor_base.h"
#include "Calculator.h"
#include "math_operations.h"

#include <iostream>
#include <regex>
#include <functional>
#include <map>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <cmath>

template<typename Key, typename Fn>
bool ProcessCommand(const std::map<Key, Fn> &commands, const Key &cmd, Processor &calc)
{
   auto p = commands.find(cmd);
   if (p == commands.end())
       return false;
   
   p->second(calc);
   return true;
}

void ShowStack(Processor &calc)
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
    
    
    std::cout << "x0   : " << calc.GetLastResult() << std::endl;
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

int main()
{
    Processor processor;
    Calculator calc;
    
    using Fn = std::function<void (Processor &)>;
    
    std::shared_ptr<CommandBase> command;
    bool doQuit = false;
        
    std::map<char, Fn> operators = 
    {
        {'+', [&command](Processor &){command = MakeCommand<MathOperator>(MathOperator::Plus);}},
        {'-', [&command](Processor &){command = MakeCommand<MathOperator>(MathOperator::Minus);}},
        {'*', [&command](Processor &){command = MakeCommand<MathOperator>(MathOperator::Mul);}},
        {'/', [&command](Processor &){command = MakeCommand<MathOperator>(MathOperator::Div);}},
    };
    
    std::map<std::string, Fn> functions = 
    {
        {"sin", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Sin);}}, 
        {"cos", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Cos);}}, 
        {"tg", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Tan);}}, 
        {"arcsin", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Asin);}}, 
        {"arccos", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Acos);}}, 
        {"arctg", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Atan);}}, 
        {"exp", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Exp);}}, 
        {"10x", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Exp10);}}, 
        {"2x", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Exp2);}}, 
        {"ln", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Ln);}}, 
        {"lg", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Lg);}}, 
        {"log2", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Log2);}}, 
        {"logyx", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Log);}}, 
        {"pow", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Pow);}}, 
        {"x2", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Sqr);}}, 
        {"sqrt", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Sqrt);}}, 
        {"1/x", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::OneByX);}}, 
        {"abs", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Abs);}}, 
        {"frac", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Frac);}}, 
        {"int", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Int);}}, 
        {"sign", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Sign);}}, 
        {"max", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Max);}}, 
        {"min", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Min);}}, 
        {"clear", [&command](Processor &){command = MakeCommand<MathFunction>(MathFunction::Clear);}}, 
        {"neg", [&command](Processor &){command = MakeCommand<MathOperator>(MathOperator::Neg);}},
        {"pi", [&command](Processor &){command = MakeCommand<LoadNumberCommand>(3.14159265358979323);}},        
    };
    
    std::map<std::string, Fn> commands = 
    {
        {"quit", [&doQuit](Processor &){doQuit = true;}}, 
        {"list", [](Processor &){std::cout << "List program" << std::endl;}}, 
        {"stack", ShowStack}, 
        {"run", [](Processor &){std::cout << "Run program" << std::endl;}}, 
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
                isProcessed = ProcessCommand(operators, result.command[0], processor);
                break;
            case 1:
                isProcessed = ProcessCommand(functions, result.command, processor);
                break;
            case 2:
                isProcessed = ProcessCommand(commands, result.command, processor);
                break;
            default:
                break;
            }
            
            if (!isProcessed)
                std::cout << "ERROR: Unrecognised input string!" << std::endl;
        }
        
        if (command)
            calc.VisitCommand(command.get(), &processor);
    }
    
    std::cout << "Well done, commander!" << std::endl;
    
    return 0;
}
