#include <Deliberation/Core/Math/MathUtils.h>

namespace deliberation
{

glm::vec3 AnyPerpendicularVectorTo(const glm::vec3 & v)
{
    if (v.x != 0.0f)
    {
        return glm::cross(v, {0.0f, 1.0f, 0.0f});
    }
    else
    {
        return glm::cross(v, {1.0f, 0.0f, 0.0f});
    }
}

glm::mat3 RotationMatrixFromDirectionY(const glm::vec3 & y)
{
    glm::mat3 result;

    result[1] = glm::normalize(y);
    result[0] = glm::normalize(AnyPerpendicularVectorTo(result[1]));
    result[2] = glm::cross(result[0], result[1]);

    return result;
}

}