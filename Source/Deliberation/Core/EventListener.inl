namespace deliberation
{

template<typename EventType, typename ReceiverType> 
std::shared_ptr<EventListener> EventListener::create(ReceiverType & receiver)
{
    return std::make_shared<EventListener>(TypeID::value<EventListener, EventType>(), [&] (const void * event) 
    {
        receiver.onEvent(*(const EventType*)event);
    });
};

}