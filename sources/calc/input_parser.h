#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <string>
#include <regex>

struct InputData
{
    double number = 0.0;
    bool isNumber = false;
    int commandType = -1;
    std::string command;
    std::string commandParam;
    std::string parseError;
};


class InputParser
{
public:
    InputParser();
    ~InputParser();
    
    InputData ParseLine(const std::string& line);
    
    void AppendRegExpString(const std::string& str)
    {
        m_regexStrings.push_back(str);
    }

    void PrepareRegExp();
    
private:
    std::string ExtractCommandParams(const std::string& line, size_t startPos);
    
    std::vector<std::string> m_regexStrings;
    std::regex m_parseRegex;
};

namespace detail 
{
template<typename Cfg, typename Map>
std::string BuildRegExpString(const Map& map, const std::string& strPrefix)
{
    std::ostringstream os;
    
    os << strPrefix;
    os << Cfg::Prefix;
    bool isFirst = true;
    for (const auto& val : map)
    {
        if (isFirst)
            isFirst = false;
        else
            os << Cfg::Delimiter;
        os << val.first;
    }
    
    os << Cfg::Suffix;
    return os.str();    
}

template<typename Key>
struct RegExpBuilderConfig;

template<>
struct RegExpBuilderConfig<char>
{
    static constexpr const char* Prefix = "[";
    static constexpr const char* Suffix = "]";
    static constexpr const char* Delimiter = "";
};

template<>
struct RegExpBuilderConfig<std::string>
{
    static constexpr const char* Prefix = "(";
    static constexpr const char* Suffix = ")";
    static constexpr const char* Delimiter = "|";
};
} // detail

template<typename Map>
void AppendRegExpString(InputParser& parser, const std::string& prefix, const Map& map)
{
    auto str = detail::BuildRegExpString<detail::RegExpBuilderConfig<typename Map::key_type>>(map, prefix);
    parser.AppendRegExpString(str);
}

#endif // INPUTPARSER_H
