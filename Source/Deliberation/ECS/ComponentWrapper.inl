namespace deliberation
{

template<typename T>
template<typename ... Args>
ComponentWrapper<T>::ComponentWrapper(Args &&... args):
    m_value(std::forward<Args>(args)...)
{

}

template<typename T>
T & ComponentWrapper<T>::value()
{
    return m_value;
}

template<typename T>
const T & ComponentWrapper<T>::value() const
{
    return m_value;
}

}