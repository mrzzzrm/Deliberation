#pragma once

#include <memory>
#include <unordered_map>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/SparseVector.h>

namespace deliberation
{

class DELIBERATION_API EventManager final
{
public:
    EventManager();

    template<typename EventType, typename ReceiverType>
    void subscribe(ReceiverType & receiver);

    template<typename EventType, typename ReceiverType>
    void unsubscribe(ReceiverType & receiver);

    template<typename EventType>
    void emit(const EventType & event);

private:
    class ICallback
    {
    public:
        virtual ~ICallback();
        virtual void receive(const void * event) = 0;
    };

private:
    std::unordered_map<std::size_t,
        SparseVector<std::unique_ptr<ICallback>>> m_receivers;
};

}

#include <Deliberation/ECS/EventManager.inl>
