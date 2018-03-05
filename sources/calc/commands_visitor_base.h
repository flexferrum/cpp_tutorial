#ifndef COMMANDS_VISITOR_BASE_H
#define COMMANDS_VISITOR_BASE_H

#include <utility>
#include <unordered_map>

#include "command_base.h"

namespace detail
{
template<size_t Idx, size_t Cur, typename ... Commands>
struct VisitorInvokator;

template<size_t Idx, size_t Cur, typename Command, typename ... Commands>
struct VisitorInvokator<Idx, Cur, Command, Commands...> : VisitorInvokator<Idx, Cur + 1, Commands ...>
{

};

template<size_t Cur, typename Command, typename ... Commands>
struct VisitorInvokator<Cur, Cur, Command, Commands ...>
{
    template<typename RT, typename V, typename ... Args>
    static RT Invoke(V&& visitor, const CommandBase* cmd, Args ... args)
    {
        return visitor.Visit(*static_cast<const Command*>(cmd), std::forward<Args>(args)...);
    }
};

template<typename Command, typename ... Commands>
struct VisitorInvokator<0, 0, Command, Commands ...>
{
    template<typename V, typename ... Args>
    static auto TestInvoke(V&& visitor, const CommandBase* cmd, Args ... args)
    {
        return visitor.Visit(*static_cast<const Command*>(cmd), std::forward<Args>(args)...);
    }

    template<typename RT, typename V, typename ... Args>
    static RT Invoke(V&& visitor, const CommandBase* cmd, Args ... args)
    {
        return TestInvoke(std::forward<V>(visitor), cmd, std::forward<Args>(args)...);
    }
};

template<size_t Idx, size_t Cur>
struct VisitorInvokator<Idx, Cur>
{
    template<typename RT, typename ... Args>
    static RT Invoke(Args&& ...)
    {
        return RT();
    }
};

template<typename IndexSeq, typename ... Commands>
struct CommandsHashGenerator;

template<size_t ... Idx, typename ... Commands>
struct CommandsHashGenerator<std::index_sequence<Idx...>, Commands ...>
{
    static int GetTypeIndex(const type_info& ti)
    {
        static std::unordered_map<size_t, int> typesMap = {
            {typeid(Commands).hash_code(), (int)Idx}...
        };


        auto p = typesMap.find(ti.hash_code());
        if (p == typesMap.end())
            return -1;

        return p->second;
    }
};
}

template<typename ... Commands>
struct CommandsList : public detail::CommandsHashGenerator<std::make_index_sequence<sizeof...(Commands)>, Commands...>
{
    template<typename ... ExtraArgs>
    static auto InvokeVisitorResult(ExtraArgs ... args)
    {
        using RT = decltype(detail::VisitorInvokator<0, 0, Commands...>::TestInvoke(std::forward<ExtraArgs>(args)...));
        return RT();
    }


    template<size_t Idx, typename V, typename ... ExtraArgs>
    static auto InvokeVisitor(V&& v, const CommandBase* cmd, ExtraArgs ... args)
    {
        using RT = decltype(InvokeVisitorResult(std::forward<V>(v), cmd, std::forward<ExtraArgs>(args)...));
        return detail::VisitorInvokator<Idx, 0, Commands...>::template Invoke<RT>(std::forward<V>(v), cmd, std::forward<ExtraArgs>(args)...);
    }

};

template<typename VT, typename CommandsList>
class CommandVisitor : CommandIdProvider
{
public:
    template<typename ... ExtraArgs>
    auto VisitCommand(const CommandBase* cmd, ExtraArgs&& ... args)
    {
        auto commandId = cmd->GetCommandId(this);
        auto& visitor = *static_cast<const VT*>(this);
        using RT = decltype(CommandsList::InvokeVisitorResult(visitor, cmd, std::forward<ExtraArgs>(args)...));

#define VISIT_COMMAND(ID) \
    case ID: \
        return CommandsList::template InvokeVisitor<ID>(visitor, cmd, std::forward<ExtraArgs>(args)...)

        switch (commandId)
        {
        VISIT_COMMAND(0);
        VISIT_COMMAND(1);
        VISIT_COMMAND(2);
        VISIT_COMMAND(3);
        VISIT_COMMAND(4);
        VISIT_COMMAND(5);
        VISIT_COMMAND(6);
        VISIT_COMMAND(7);
        VISIT_COMMAND(8);
        VISIT_COMMAND(9);
        VISIT_COMMAND(10);
        VISIT_COMMAND(11);
        VISIT_COMMAND(12);
        VISIT_COMMAND(13);
        VISIT_COMMAND(14);
        VISIT_COMMAND(15);
        VISIT_COMMAND(16);
        VISIT_COMMAND(17);
        VISIT_COMMAND(18);
        VISIT_COMMAND(19);
        VISIT_COMMAND(20);
        VISIT_COMMAND(21);
        VISIT_COMMAND(22);
        VISIT_COMMAND(23);
        VISIT_COMMAND(24);
        VISIT_COMMAND(25);
        VISIT_COMMAND(26);
        VISIT_COMMAND(27);
        VISIT_COMMAND(28);
        VISIT_COMMAND(29);
        VISIT_COMMAND(30);
        VISIT_COMMAND(31);
        VISIT_COMMAND(32);
        VISIT_COMMAND(33);
        VISIT_COMMAND(34);
        VISIT_COMMAND(35);
        VISIT_COMMAND(36);
        VISIT_COMMAND(37);
        VISIT_COMMAND(38);
        VISIT_COMMAND(39);
        VISIT_COMMAND(40);
        VISIT_COMMAND(41);
        VISIT_COMMAND(42);
        VISIT_COMMAND(43);
        VISIT_COMMAND(44);
        VISIT_COMMAND(45);
        VISIT_COMMAND(46);
        VISIT_COMMAND(47);
        VISIT_COMMAND(48);
        VISIT_COMMAND(49);
        VISIT_COMMAND(50);
        VISIT_COMMAND(51);
        VISIT_COMMAND(52);
        VISIT_COMMAND(53);
        VISIT_COMMAND(54);
        VISIT_COMMAND(55);
        VISIT_COMMAND(56);
        VISIT_COMMAND(57);
        VISIT_COMMAND(58);
        VISIT_COMMAND(59);
        VISIT_COMMAND(60);
        VISIT_COMMAND(61);
        VISIT_COMMAND(62);
        VISIT_COMMAND(63);
        VISIT_COMMAND(64);
        VISIT_COMMAND(65);
        VISIT_COMMAND(66);
        VISIT_COMMAND(67);
        VISIT_COMMAND(68);
        VISIT_COMMAND(69);
        default:
            break;
        };

        return RT();
    }

    int GetCommandId(const type_info &ti) const override
    {
        return CommandsList::GetTypeIndex(ti);
    }
};

#endif // COMMANDS_VISITOR_BASE_H
