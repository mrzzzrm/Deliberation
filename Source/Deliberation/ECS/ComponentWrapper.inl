namespace deliberation
{
template<typename WrappedType, typename ComponentType>
template<typename... Args>
ComponentWrapper<WrappedType, ComponentType>::ComponentWrapper(Args &&... args)
    : m_value(std::forward<Args>(args)...)
{
}

template<typename WrappedType, typename ComponentType>
WrappedType & ComponentWrapper<WrappedType, ComponentType>::value()
{
    return m_value;
}

template<typename WrappedType, typename ComponentType>
const WrappedType & ComponentWrapper<WrappedType, ComponentType>::value() const
{
    return m_value;
}
}