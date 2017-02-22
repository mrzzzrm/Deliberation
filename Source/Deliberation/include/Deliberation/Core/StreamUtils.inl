#include <iostream>
#include <sstream>

namespace deliberation
{

template<typename StreamType, typename T, glm::precision precision>
StreamType & operator<<(StreamType && os, const glm::tvec2<T, precision> & v)
{
    os << "{" << std::to_string(v.x) << "," << std::to_string(v.y) << "}";
    return os;
}

template<typename StreamType, typename T, glm::precision precision>
StreamType & operator<<(StreamType && os, const glm::tvec3<T, precision> & v)
{
    os << "{" << std::to_string(v.x) << "," << std::to_string(v.y) << "," << std::to_string(v.z) << "}";
    return os;
}

template<typename StreamType, typename T, glm::precision precision>
StreamType & operator<<(StreamType && os, const glm::tvec4<T, precision> & v)
{
    os << "{" << std::to_string(v.x) << "," << std::to_string(v.y) << "," <<
        std::to_string(v.z) << "," << std::to_string(v.w) << "}";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::quat & q)
{
    os << "{"  << q.w << "," << q.x << "," << q.y << "," << q.z << "}";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::mat2 & v)
{
    os << "{" << std::endl;

    for (auto r = 0u; r < 2u; r++)
    {
        os << "  ";
        for (auto c = 0u; c < 2u; c++)
        {
            os << v[c][r] << ",";
        }
        os << std::endl;
    }

    os << "}" << std::endl;

    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::mat3 & v)
{
    os << "{" << std::endl;

    for (auto r = 0u; r < 3u; r++)
    {
        os << "  ";
        for (auto c = 0u; c < 3u; c++)
        {
            os << v[c][r] << ",";
        }
        os << std::endl;
    }

    os << "}" << std::endl;

    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::mat4 & v)
{
    os << "{" << std::endl;

    for (auto r = 0u; r < 4u; r++)
    {
        os << "  ";
        for (auto c = 0u; c < 4u; c++)
        {
            os << v[c][r] << ",";
        }
        os << std::endl;
    }

    os << "}" << std::endl;

    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const AABB & aabb)
{
    os << "{" << aabb.llf() << " -> " << aabb.urb() << "}";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const Ray3D & r)
{
    os << "{" << r.origin() << " -> " << r.direction() << "}";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const Ray2D & r)
{
    os << "{" << r.origin() << " -> " << r.direction() << "}";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const Sphere & s)
{
    os << "{" << s.position() << " -> " << s.radius() << "}";
    return os;
}

template<typename T>
std::string operator+(const std::string & s, const T & value)
{
    std::stringstream stream;
    stream << s << value;
    return stream.str();
}

template<typename T>
std::string operator+(const T & value, const std::string & s)
{
    std::stringstream stream;
    stream << value << s;
    return stream.str();
}
//
//template<typename T>
//std::string operator+(const char * s, const T & value)
//{
//    std::stringstream stream;
//    stream << s << value;
//    return stream.str();
//}
//
//template<typename T>
//std::string operator+(const T & value, const char * s)
//{
//    std::stringstream stream;
//    stream << value << s;
//    return stream.str();
//}

}

