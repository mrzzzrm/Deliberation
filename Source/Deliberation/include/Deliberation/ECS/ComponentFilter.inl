namespace deliberation
{

namespace detail
{

template<typename ... Args>
struct SetComponentBits;

template<typename Head, typename ... Args>
struct SetComponentBits<Head, Args...>
{
    static void unpack(ComponentBitset & bits)
    {
        bits.set(Head::index);
        SetComponentBits<Args...>::unpack(bits);
    }
};

template<>
struct SetComponentBits<>
{
    static void unpack(ComponentBitset & bits)
    {
    }
};

}

template<typename ... Args>
ComponentFilter::ComponentFilter()
{
    detail::SetComponentBits<Args...>::unpack(m_components);
}

}


