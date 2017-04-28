#include <atomic>

namespace deliberation
{

namespace detail
{

struct TypeIDDummy {};

template<typename Family>
struct TypeIDCounter
{
public:
    static std::atomic<TypeID::value_t> s_counter;
};

template<typename Family> std::atomic<TypeID::value_t> TypeIDCounter<Family>::s_counter(0);

}

template<typename T>
TypeID::value_t TypeID::value()
{
    static TypeID::value_t value = generate<detail::TypeIDDummy, T>();
    return value;
}

template<typename Family, typename T>
TypeID::value_t TypeID::value()
{
    static TypeID::value_t value = generate<Family, T>();
    return value;
}

template<typename Family, typename T>
TypeID::value_t TypeID::generate()
{
    return std::atomic_fetch_add(&detail::TypeIDCounter<Family>::s_counter, 1u);
}

}

