#include <Deliberation/Core/Math/GLMUtils.h>

#include <cmath>

namespace deliberation
{

bool GLMIsFinite(const glm::vec2 & v)
{
    return std::isfinite(v.x) && std::isfinite(v.y);
}

bool GLMIsFinite(const glm::vec3 & v)
{
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
}

bool GLMIsFinite(const glm::vec4 & v)
{
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z) && std::isfinite(v.w);
}

bool GLMIsFinite(const glm::quat & q)
{
    return std::isfinite(q.x) && std::isfinite(q.y) && std::isfinite(q.z) && std::isfinite(q.w);
}

bool GLMVec3Compare::operator()(const glm::vec3 & a, const glm::vec3 & b) const
{
    if (a.x != b.x)
    {
        return a.x < b.x;
    }
    else if (a.y != b.y)
    {
        return a.y < b.y;
    }
    else
    {
        return a.z < b.z;
    }
}

}

