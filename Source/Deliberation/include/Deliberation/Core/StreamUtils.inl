namespace deliberation
{

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::uvec2 & v)
{
    os << "(" << v.x << "/" << v.y << ")";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::uvec3 & v)
{
    os << "(" << v.x << "/" << v.y << "/" << v.z << ")";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::uvec4 & v)
{
    os << "(" << v.x << "/" << v.y << "/" << v.z << "/" << v.w << ")";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::vec2 & v)
{
    os << "(" << v.x << "/" << v.y << ")";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::vec3 & v)
{
    os << "(" << v.x << "/" << v.y << "/" << v.z << ")";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::vec4 & v)
{
    os << "(" << v.x << "/" << v.y << "/" << v.z << "/" << v.w << ")";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::quat & q)
{
    os << "(" << q.x << "/" << q.y << "/" << q.z << "/" << q.w << ")";
    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::mat3 & v)
{
    os << "(" << std::endl;

    for (auto r = 0u; r < 3u; r++)
    {
        os << "  ";
        for (auto c = 0u; c < 3u; c++)
        {
            os << v[c][r] << "/";
        }
        os << std::endl;
    }

    os << ")" << std::endl;

    return os;
}

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::mat4 & v)
{
    os << "(" << std::endl;

    for (auto r = 0u; r < 4u; r++)
    {
        os << "  ";
        for (auto c = 0u; c < 4u; c++)
        {
            os << v[c][r] << "/";
        }
        os << std::endl;
    }

    os << ")" << std::endl;

    return os;
}

}
