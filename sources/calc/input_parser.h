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
    static bool ParseNumber(const std::string& str, double& num);
    
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
    
    std::vector<typename Map::key_type> keys;
    std::transform(map.begin(), map.end(), std::back_inserter(keys), [](auto& v) {return v.first;});
    std::sort(keys.begin(), keys.end(), [](auto& k1, auto& k2) {return Cfg::KeySize(k1) > Cfg::KeySize(k2);});
    for (const auto& val : keys)
    {
        if (isFirst)
            isFirst = false;
        else
            os << Cfg::Delimiter;
        Cfg::Escape(os, val);
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
    static size_t KeySize(char) {return 1;}
    static auto Escape(std::ostream& os, char ch)
    {
        os << '\\' << ch;
    }
};

template<>
struct RegExpBuilderConfig<std::string>
{
    static constexpr const char* Prefix = "(";
    static constexpr const char* Suffix = ")";
    static constexpr const char* Delimiter = "|";
    static size_t KeySize(const std::string& key) {return key.size();}
    static auto Escape(std::ostream& os, const std::string& str)
    {
        os << str;
    }
};
} // detail

template<typename Map>
void AppendRegExpString(InputParser& parser, const std::string& prefix, const Map& map)
{
    auto str = detail::BuildRegExpString<detail::RegExpBuilderConfig<typename Map::key_type>>(map, prefix);
    parser.AppendRegExpString(str);
}

#endif // INPUTPARSER_H
