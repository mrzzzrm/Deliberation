#include <iostream>
#include <sstream>

template<typename T, glm::precision precision>
std::ostream & operator<<(std::ostream & os, const glm::tvec2<T, precision> & v)
{
    os << "{" << std::to_string(v.x) << "," << std::to_string(v.y) << "}";
    return os;
}

template<typename T, glm::precision precision>
std::ostream & operator<<(std::ostream & os, const glm::tvec3<T, precision> & v)
{
    os << "{" << std::to_string(v.x) << "," << std::to_string(v.y) << ","
       << std::to_string(v.z) << "}";
    return os;
}

template<typename T, glm::precision precision>
std::ostream & operator<<(std::ostream & os, const glm::tvec4<T, precision> & v)
{
    os << "{" << std::to_string(v.x) << "," << std::to_string(v.y) << ","
       << std::to_string(v.z) << "," << std::to_string(v.w) << "}";
    return os;
}


// template<typename T>
// std::string operator+(const std::string & s, const T & value)
//{
//    std::stringstream stream;
//    stream << s << value;
//    return stream.str();
//}
//
// template<typename T>
// std::string operator+(const T & value, const std::string & s)
//{
//    std::stringstream stream;
//    stream << value << s;
//    return stream.str();
//}
//
// template<typename T>
// std::string operator+(const char * s, const T & value)
//{
//    std::stringstream stream;
//    stream << s << value;
//    return stream.str();
//}
//
// template<typename T>
// std::string operator+(const T & value, const char * s)
//{
//    std::stringstream stream;
//    stream << value << s;
//    return stream.str();
//}

namespace deliberation
{
template<typename T, glm::precision precision>
std::string ToString(const glm::tvec2<T, precision> & v)
{
    std::stringstream stream;
    stream << v;
    return stream.str();
}

template<typename T, glm::precision precision>
std::string ToString(const glm::tvec3<T, precision> & v)
{
    std::stringstream stream;
    stream << v;
    return stream.str();
}

template<typename T, glm::precision precision>
std::string ToString(const glm::tvec4<T, precision> & v)
{
    std::stringstream stream;
    stream << v;
    return stream.str();
}
}
