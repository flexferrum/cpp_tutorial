#include "processor.h"
#include "input_parser.h"
#include "command_base.h"
#include "commands_visitor_base.h"

#include <iostream>
#include <regex>
#include <functional>
#include <map>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <cmath>

class LoadNumberCommand : public CommandBase
{
public:
    LoadNumberCommand(double num)
    {
        ;
    }
};

class UnaryOperationCommand : public CommandBase
{
public:
};

class BinaryOperationCommand : public CommandBase
{
public:
};

using Commands = CommandsList<LoadNumberCommand, UnaryOperationCommand, BinaryOperationCommand>;

class Calculator : public CommandVisitor<Calculator, Commands>
{
public:
    void Visit(const LoadNumberCommand& cmd, const Processor& proc)
    {
        std::cout << "LoadNumberCommand visited" << std::endl;
    }
    void Visit(const UnaryOperationCommand& cmd, const Processor& proc)
    {
        std::cout << "UnaryOperationCommand visited" << std::endl;
    }
    void Visit(const BinaryOperationCommand& cmd, const Processor& proc)
    {
        std::cout << "BinaryOperationCommand visited" << std::endl;
    }
};

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

int main()
{
    Processor calc;
    
    using Fn = std::function<void (Processor &)>;
    
    std::map<char, Fn> operators = 
    {
        {'+', [](Processor &calc){DoBinaryOper(calc, std::plus<double>());}},
        {'-', [](Processor &calc){DoBinaryOper(calc, std::minus<double>());}},
        {'*', [](Processor &calc){DoBinaryOper(calc, std::multiplies<double>());}},
        {'/', [](Processor &calc){DoBinaryOper(calc, 
                                   [](double x, double y) {return y / x;}, 
                                   [](double x, double /*y*/) {return x != 0;});
         }},
    };
    
    std::map<std::string, Fn> functions = 
    {
        {"sin", [](Processor &calc){DoUnaryOper(calc, sin);}}, 
        {"cos", [](Processor &calc){DoUnaryOper(calc, cos);}}, 
        {"exp", [](Processor &calc){DoUnaryOper(calc, exp);}}, 
        {"pow", [](Processor &calc){DoBinaryOper(calc, pow);}}, 
        {"neg", [](Processor &calc){DoUnaryOper(calc, [](double val) {return -val;});}}, 
    };
    
    bool doQuit = false;
    
    std::map<std::string, Fn> commands = 
    {
        {"quit", [&doQuit](Processor &){doQuit = true;}}, 
        {"list", [](Processor &){std::cout << "List program" << std::endl;}}, 
        {"stack", ShowStack}, 
        {"run", [](Processor &){std::cout << "Run program" << std::endl;}}, 
    };
    
    LoadNumberCommand cmd1(10.0);
    UnaryOperationCommand cmd2;
    BinaryOperationCommand cmd3;
    
    Calculator calcNew;
    
    calcNew.VisitCommand(&cmd1, calc);
    calcNew.VisitCommand(&cmd2, calc);
    calcNew.VisitCommand(&cmd3, calc);
   
    InputParser inputParser;
    AppendRegExpString(inputParser, "", operators);
    AppendRegExpString(inputParser, "?:", functions);
    AppendRegExpString(inputParser, "?:#", commands);
    inputParser.PrepareRegExp();
    
    while (!doQuit)
    {
        std::cout << "> ";
        std::cout.flush();
        std::string line;
        std::getline(std::cin, line);
        
        auto result = inputParser.ParseLine(line);
        
//      DumpInputData(result);
        
        if (!result.parseError.empty())
        {
            std::cout << "ERROR: " << result.parseError << std::endl;
            continue;
        }
        
        if (result.isNumber)
        {
            calc.Push(result.number, false);
            continue;
        }
        
        bool isProcessed = false;
        switch (result.commandType)
        {
        case 0:
            isProcessed = ProcessCommand(operators, result.command[0], calc);
            break;
        case 1:
            isProcessed = ProcessCommand(functions, result.command, calc);
            break;
        case 2:
            isProcessed = ProcessCommand(commands, result.command, calc);
            break;
        default:
            break;
        }
        
        if (!isProcessed)
            std::cout << "ERROR: Unrecognised input string!" << std::endl;
    }
    
    std::cout << "Well done, commander!" << std::endl;
    
    return 0;
}
