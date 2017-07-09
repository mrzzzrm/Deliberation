namespace deliberation
{

template<typename EventType, typename ReceiverType> 
std::shared_ptr<EventListenerProxy> EventListenerProxy::create(ReceiverType & receiver)
{
    return std::make_shared<EventListenerProxy>(TypeID::value<EventListenerProxy, EventType>(), [&] (const void * event)
    {
        receiver.onEvent(*(const EventType*)event);
    });
};

}