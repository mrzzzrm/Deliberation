#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <Deliberation/Core/UpdateFrame.h>
#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

class AbstractPhaseContainer;

class AbstractPhaseInvoker
{
public:
    virtual ~AbstractPhaseInvoker() = default;
    virtual TypeID::value_t phaseTypeId() = 0;
    virtual std::shared_ptr<AbstractPhaseContainer> createContainer() = 0;
};

class AbstractPhaseContainer
{
public:
    virtual ~AbstractPhaseContainer() = default;
    virtual void addInvoker(const std::shared_ptr<AbstractPhaseInvoker> & invoker) = 0;
    virtual void removeInvoker(const std::shared_ptr<AbstractPhaseInvoker> & invoker) = 0;
};

template<typename T, typename ... Args>
class Phase
{
public:
    using Fn = std::function<void(Args &...)>;

public:
    class Invoker : public AbstractPhaseInvoker
    {
    public:
        Invoker(const Fn & fn):
            fn(fn)
        {}

        TypeID::value_t phaseTypeId() override
        {
            return T::phaseTypeId();
        }

        std::shared_ptr<AbstractPhaseContainer> createContainer() override;
        Fn fn;
    };

    class Container : public AbstractPhaseContainer
    {
    public:
        std::vector<std::shared_ptr<Invoker>> invokers;

        void addInvoker(const std::shared_ptr<AbstractPhaseInvoker> & abstractInvoker) override
        {
            const auto invoker = std::static_pointer_cast<Invoker>(abstractInvoker);
            invokers.emplace_back(invoker);
        }

        void removeInvoker(const std::shared_ptr<AbstractPhaseInvoker> & abstractInvoker) override
        {
            const auto invoker = std::static_pointer_cast<Invoker>(abstractInvoker);
            const auto iter = std::find(invokers.begin(), invokers.end(), invoker);
            Assert(iter != invokers.end());
            invokers.erase(iter);
        }

        template<typename ... Args2>
        void invokeAll(Args2 &&... args)
        {
            for (const auto & invoker : invokers)
            {
                invoker->fn(std::forward<Args2>(args)...);
            }
        }
    };
};

template<typename T, typename ... Args>
std::shared_ptr<AbstractPhaseContainer> Phase<T, Args...>::Invoker::createContainer()
{
    return std::make_shared<typename T::Container>();
};


class GameUpdatePhase:
    public Phase<GameUpdatePhase, UpdateFrame>
{
public:
    static TypeID::value_t phaseTypeId();

    template<typename T>
    static std::shared_ptr<Invoker> createInvoker(T & obj)
    {
        Fn fn = [&](UpdateFrame & updateFrame) {
            obj.onGameUpdate(updateFrame);
        };
        return std::make_shared<Invoker>(fn);
    }
};

}