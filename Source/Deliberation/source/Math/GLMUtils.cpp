#include <Deliberation/Math/GLMUtils.h>

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

}

