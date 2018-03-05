#ifndef COMMAND_BASE_H
#define COMMAND_BASE_H

#include <memory>
#include <typeinfo>
#include <string>

class Processor;

class CommandIdProvider
{
public:
    virtual int GetCommandId(const type_info& ti) const = 0;
};

class CommandBase
{
public:
    virtual ~CommandBase() {}

    virtual int GetCommandId(const CommandIdProvider* idProv) const
    {
        return idProv->GetCommandId(typeid(*this));
    }

    void SetLabel(const std::string &label)
    {
        m_label = label;
    }

    auto& GetLabel() const
    {
        return m_label;
    }

    virtual void Print() const = 0;

private:
    std::string m_label;
};

#endif // COMMAND_BASE_H
