#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

template<typename T>
std::size_t Component<T>::indexStatic()
{
    return TypeID::value<T>();
}

template<typename T>
std::size_t Component<T>::index() const
{
    return indexStatic();
}

template<typename T>
std::string Component<T>::name() const
{
    return typeid(T).name();
}

}

