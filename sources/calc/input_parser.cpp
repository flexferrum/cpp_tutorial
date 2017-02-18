#include "input_parser.h"

#include <cstdlib>
#include <cctype>
#include <iostream>

InputParser::InputParser()
{
    
}

InputParser::~InputParser()
{
    
}

InputData InputParser::ParseLine(const std::string &line)
{
    InputData result;
    
    if (ParseNumber(line, result.number))
    {
        result.isNumber = true;
        return result;
    }
    
    auto matchBegin = std::sregex_iterator(line.begin(), line.end(), m_parseRegex);
    auto matchEnd = std::sregex_iterator();
    
    auto matches = std::distance(matchBegin, matchEnd);
    if (matches == 0)
    {
        result.parseError = "Wrong input string!";
        return result;
    }
    
    if (matches > 1)
    {
        result.parseError = "Ambigous input!";
        return result;
    }
    
    auto &match = *matchBegin;
    
    for (size_t idx = 1; idx < match.size(); ++ idx)
    {
        if (match.length(idx) != 0)
        {
            result.command = match.str(idx);
            result.commandType = idx - 1;
            result.commandParam = ExtractCommandParams(line, result.command.length());
            break;
        }
    }
    
    return result;
}

std::string InputParser::ExtractCommandParams(const std::string &line, size_t startPos)
{
    auto p = line.begin() + startPos;
    for (; p != line.end(); ++ p)
        if (!std::isspace(*p))
            break;
    
    return std::string(p, line.end());
}

void InputParser::PrepareRegExp()
{
    std::string resultStr;
    
    for (auto& str : m_regexStrings)
    {
        if (!resultStr.empty())
            resultStr += "|";
        resultStr += "(" + str + ")";
    }
    
    std::cout << "Result regexp: " << resultStr << std::endl;
    
    m_parseRegex = std::regex(resultStr);
}

bool InputParser::ParseNumber(const std::string &str, double &num)
{
    const char *numBeg = str.c_str();
    char *numEnd = nullptr;
    num = strtod(numBeg, &numEnd);
    if (numEnd != numBeg && (*numEnd == '\0' || std::isspace(*numEnd)))
        return true;
    
    return false;
}

