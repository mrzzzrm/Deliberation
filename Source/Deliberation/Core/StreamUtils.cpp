#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Core/Math/AABB.h>
#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Core/Math/Ray2D.h>
#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/Math/Sphere.h>
#include <Deliberation/Core/Math/Transform3D.h>

std::ostream & operator<<(std::ostream & os, const glm::quat & q)
{
    os << "{" << q.w << "," << q.x << "," << q.y << "," << q.z << "}";
    return os;
}

std::ostream & operator<<(std::ostream & os, const glm::mat2 & v)
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

std::ostream & operator<<(std::ostream & os, const glm::mat3 & v)
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

std::ostream & operator<<(std::ostream & os, const glm::mat4 & v)
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
