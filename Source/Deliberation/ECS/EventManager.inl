#include <Deliberation/Core/TypeID.h>

namespace deliberation
{
template<typename EventType, typename ReceiverType>
void EventManager::subscribe(ReceiverType & receiver)
{
    class Callback : public ICallback
    {
      public:
        Callback(ReceiverType & receiver) : m_receiver(receiver) {}

        virtual void receive(const void * event) override
        {
            m_receiver.receive(*(const EventType *)event);
        }

      private:
        ReceiverType & m_receiver;
    };

    auto * callback = new Callback(receiver);
    auto & receivers = m_receivers[TypeID::value<EventManager, EventType>()];

    receivers.insert(std::unique_ptr<ICallback>(callback));
}

template<typename EventType, typename ReceiverType>
void EventManager::unsubscribe(ReceiverType & receiver)
{
}

template<typename EventType>
void EventManager::emit(const EventType & event)
{
    auto & receivers = m_receivers[TypeID::value<EventManager, EventType>()];

    for (auto & receiver : receivers)
    {
        receiver->receive(&event);
    }
}
}
