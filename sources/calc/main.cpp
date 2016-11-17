#include <iostream>
#include <regex>
#include <functional>
#include <map>
#include <string>
#include <sstream>
#include <deque>
#include <cstdlib>
#include <cctype>
#include <cmath>

class Calculator
{
public:
    void Push(double val, bool updateLastRes)
    {
        if (updateLastRes)
            m_lastResult = val;
        
        m_stack.push_front(val);
    }
    
    bool Top(double &x) const
    {
        if (m_stack.empty())
            return false;
        
        x = m_stack[0];
        return true;
    }
    
    void Pop()
    {
        if (!m_stack.empty())
            m_stack.pop_front();
    }
    
    bool TopPair(double &x, double &y) const
    {
        if (m_stack.size() < 2)
            return false;
        
        x = m_stack[0];
        y = m_stack[1];
        return true;
    }
    
    void PopPair()
    {
        Pop();
        Pop();
    }
    
    const auto &GetStack() const
    {
        return m_stack;
    }
    
    auto GetLastResult() const
    {
        return m_lastResult;
    }
    
private:
    double m_lastResult = 0;
    std::deque<double> m_stack;
    std::vector<double> m_registers;
    // const int m_maxStackSize = 4;
};
        
template<typename T>
std::string BuildSubRegexpImpl(const std::vector<T> &keys);

template<>
std::string BuildSubRegexpImpl(const std::vector<char> &keys)
{
    std::ostringstream os;
    os << '[';
    for (char key : keys)
        os << key;
    
    os << ']';
    return os.str();
}

template<>
std::string BuildSubRegexpImpl(const std::vector<std::string> &keys)
{
    std::ostringstream os;
    
    os << '(';
    bool isFirst = true;
    for (const std::string& key : keys)
    {
        if (isFirst)
            isFirst = false;
        else
            os << '|';
        os << key;
    }
    
    os << ')';
    return os.str();
}

template<typename Map>
auto BuildSubRegexp(const Map &map)
{
    using KeyType = typename Map::key_type;
    std::vector<typename Map::key_type> keys;
    keys.reserve(map.size());
    for (auto &i : map)
        keys.push_back(i.first);
    
    return BuildSubRegexpImpl<KeyType>(keys);
}

struct NoCheck
{
    constexpr bool operator()(double /*x*/) const {return true;}
    constexpr bool operator()(double /*x*/, double /*y*/) const {return true;}
};

template<typename Fn = double (double, double), typename Checker = NoCheck>
void DoBinaryOper(Calculator &calc, Fn oper, Checker checker = Checker())
{
    double x = 0;
    double y = 0;
    if (!calc.TopPair(x, y))
        return;
    
    if (!checker(x, y))
    {
        std::cout << "ERROR!" << std::endl;
        return;
    }
    
    auto val = oper(x, y);
    calc.PopPair();
    calc.Push(val, true);
    std::cout << val << std::endl;    
}

template<typename Fn = double (double), typename Checker = NoCheck>
void DoUnaryOper(Calculator &calc, Fn oper, Checker checker = Checker())
{
    double x = 0;
    if (!calc.Top(x))
        return;
    
    if (!checker(x))
    {
        std::cout << "ERROR!" << std::endl;
        return;
    }
    
    auto val = oper(x);
    calc.PopPair();
    calc.Push(val, true);
    std::cout << val << std::endl;    
}

template<typename Key, typename Fn>
bool ProcessCommand(const std::map<Key, Fn> &commands, const Key &cmd, Calculator &calc)
{
   auto p = commands.find(cmd);
   if (p == commands.end())
       return false;
   
   p->second(calc);
   return true;
}

void ShowStack(Calculator &calc)
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

int main()
{
    Calculator calc;
    
    using Fn = std::function<void (Calculator &)>;
    
    std::map<char, Fn> operators = 
    {
        {'+', [](Calculator &calc){DoBinaryOper(calc, std::plus<double>());}},
        {'-', [](Calculator &calc){DoBinaryOper(calc, std::minus<double>());}},
        {'*', [](Calculator &calc){DoBinaryOper(calc, std::multiplies<double>());}},
        {'/', [](Calculator &calc){DoBinaryOper(calc, std::divides<double>(), [](double /*x*/, double y) {return y != 0;});}},
    };
    
    std::map<std::string, Fn> functions = 
    {
        {"sin", [](Calculator &calc){DoUnaryOper(calc, sin);}}, 
        {"cos", [](Calculator &calc){DoUnaryOper(calc, cos);}}, 
        {"exp", [](Calculator &calc){DoUnaryOper(calc, exp);}}, 
        {"pow", [](Calculator &calc){DoBinaryOper(calc, pow);}}, 
        {"neg", [](Calculator &calc){DoUnaryOper(calc, [](double val) {return -val;});}}, 
    };
    
    bool doQuit = false;
    
    std::map<std::string, Fn> commands = 
    {
        {"quit", [&doQuit](Calculator &){doQuit = true;}}, 
        {"list", [](Calculator &){std::cout << "List program" << std::endl;}}, 
        {"stack", ShowStack}, 
        {"run", [](Calculator &){std::cout << "Run program" << std::endl;}}, 
    };    
    
    std::string expr = 
            "(" + BuildSubRegexp(operators) + ")|(?:"
            + BuildSubRegexp(functions) + ")|(?:#"
            + BuildSubRegexp(commands) + ")";
    
    std::cout << expr << std::endl;
    
    std::regex regexParser(expr);
    
    while (!doQuit)
    {
        std::cout << "> ";
        std::cout.flush();
        std::string line;
        std::getline(std::cin, line);
        
        const char *numBeg = line.c_str();
        char *numEnd = nullptr;
        double val = strtod(numBeg, &numEnd);
        if (numEnd != numBeg && (*numEnd == '\0' || std::isspace(*numEnd)))
        {
            calc.Push(val, false);
            continue;
        }
        
        auto matchBegin = std::sregex_iterator(line.begin(), line.end(), regexParser);
        auto matchEnd = std::sregex_iterator();
        
        auto matches = std::distance(matchBegin, matchEnd);
        if (matches == 0)
        {
            std::cout << "Wrong input string!" << std::endl;
            continue;
        }
        
        if (matches > 1)
        {
            std::cout << "Ambigous input!" << std::endl;
            continue;
        }
        
        auto &match = *matchBegin;
        
        bool isProcessed = false;
        if (match[1].length() != 0)
            isProcessed = ProcessCommand(operators, match[1].str()[0], calc);
        else if (match[2].length() != 0)
            isProcessed = ProcessCommand(functions, match[2].str(), calc);
        else if (match[3].length() != 0)
            isProcessed = ProcessCommand(commands, match[3].str(), calc);
        
        if (!isProcessed)
            std::cout << "Unrecognized input!" << std::endl;
    }
    
    return 0;
}
