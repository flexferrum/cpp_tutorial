#ifndef COMMAND_BASE_H
#define COMMAND_BASE_H

#include <memory>
#include <typeinfo>

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
};

#endif // COMMAND_BASE_H
