#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <Deliberation/Core/Assert.h>
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

// Used to generate Family of Phase Type IDs
struct PhaseFamily {};

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

public:
    static TypeID::value_t phaseTypeId() {
        return TypeID::value<PhaseFamily, T>();
    }
};

template<typename T, typename ... Args>
std::shared_ptr<AbstractPhaseContainer> Phase<T, Args...>::Invoker::createContainer()
{
    return std::make_shared<typename T::Container>();
};

#define DELIBERATION_DECLARE_PHASE_0(phaseName, methodName) \
class phaseName: \
    public Phase<phaseName> \
{ \
public:\
    template<typename T> \
    static std::shared_ptr<Invoker> createInvoker(T & obj) \
    { \
        Fn fn = [&]() { \
            obj.methodName(); \
        }; \
        return std::make_shared<Invoker>(fn); \
    } \
};

#define DELIBERATION_DECLARE_PHASE_1(phaseName, methodName, arg0) \
class phaseName: \
    public Phase<phaseName, arg0> \
{ \
public:\
    template<typename T> \
    static std::shared_ptr<Invoker> createInvoker(T & obj) \
    { \
        Fn fn = [&](arg0 & v0) { \
            obj.methodName(v0); \
        }; \
        return std::make_shared<Invoker>(fn); \
    } \
}; \

DELIBERATION_DECLARE_PHASE_0(FrameBeginPhase, onFrameBegin)
DELIBERATION_DECLARE_PHASE_1(GameUpdatePhase, onGameUpdate, const UpdateFrame)
DELIBERATION_DECLARE_PHASE_1(FrameUpdatePhase, onFrameUpdate, const UpdateFrame)
DELIBERATION_DECLARE_PHASE_1(PrePhysicsUpdatePhase, onPrePhysicsUpdate, const UpdateFrame)
DELIBERATION_DECLARE_PHASE_1(PostPhysicsUpdatePhase, onPostPhysicsUpdate, const UpdateFrame)
DELIBERATION_DECLARE_PHASE_1(FrameCompletePhase, onFrameComplete, const UpdateFrame)

}