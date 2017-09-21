namespace deliberation
{
template<typename EventType, typename ReceiverType>
std::shared_ptr<EventListenerProxy>
EventListenerProxy::create(ReceiverType & receiver, void (ReceiverType::*handlerFn)(const EventType &))
{
    return std::make_shared<EventListenerProxy>(
        TypeID::value<EventListenerProxy, EventType>(),
        [&](const void * event) {
            (receiver.*handlerFn)(*(const EventType *)event);
        });
};
}