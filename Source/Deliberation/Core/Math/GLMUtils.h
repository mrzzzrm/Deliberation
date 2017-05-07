#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace deliberation
{
bool GLMIsFinite(const glm::vec2 & v);
bool GLMIsFinite(const glm::vec3 & v);
bool GLMIsFinite(const glm::vec4 & v);
bool GLMIsFinite(const glm::quat & q);

struct GLMVec3Compare
{
    bool operator()(const glm::vec3 & a, const glm::vec3 & b) const;
};
}
