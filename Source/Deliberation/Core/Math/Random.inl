#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/FloatUtils.h>

namespace deliberation
{
bool RandomBool(float probability)
{
    auto threshold = probability * float(RAND_MAX);
    return rand() < threshold;
}

int RandomInt(int from, int to)
{
    Assert(from <= to);
    return from + std::rand() % (to - from);
}

float RandomFloat(float from, float to)
{
    Assert(from <= to);
    return from + (float)std::rand() / (float)(RAND_MAX / (to - from));
}

glm::vec2 RandomVec2(float from, float to)
{
    return glm::vec2(RandomFloat(from, to), RandomFloat(from, to));
}

glm::vec3 RandomVec3(float from, float to)
{
    return glm::vec3(
        RandomFloat(from, to), RandomFloat(from, to), RandomFloat(from, to));
}

glm::vec3 RandomUnitVec3()
{
    float angX = RandomFloat(-glm::pi<float>(), glm::pi<float>());
    float angY = glm::acos(RandomFloat(-1.0f, 1.0f));

    glm::vec3 result =
        glm::quat(glm::vec3(angX, angY, 0.0f)) * glm::vec3(0.0f, 0.0f, 1.0f);

    return glm::normalize(result);
}

glm::vec2 RandomUnitVec2()
{
    float angZ = RandomFloat(-glm::pi<float>(), glm::pi<float>());

    glm::vec3 result =
        glm::quat(glm::vec3(0.0f, 0.0f, angZ)) * glm::vec3(0.0f, 1.0f, 0.0f);

    return glm::vec2(result.x, result.y);
}

glm::vec3 RandomInSphere()
{
    glm::vec3 p;

    do
    {
        p = RandomVec3(-1.0f, 1.0f);
    } while (p.x * p.x + p.y * p.y + p.z * p.z > 1.0f); // Yes, seriously

    return p;
}

glm::vec2 RandomOnCircle()
{
    glm::vec2 p;

    do
    {
        p = RandomVec2(-1.0f, 1.0f);
    } while (p.x * p.x + p.y * p.y > 1.0f); // Yes, seriously

    return p;
}

glm::vec3 RandomInHemisphere(const glm::vec3 & normal)
{
    glm::vec3 p;

    do
    {
        p = RandomInSphere();
    } while (glm::dot(p, normal) < 0.0f); // Not kidding, no...

    return p * glm::length(normal);
}

glm::vec3 RandomOnHemisphere(const glm::vec3 & normal)
{
    glm::vec3 p;

    do
    {
        p = RandomInHemisphere(normal);
    } while (EpsilonEq(glm::dot(p, p), 0.0f)); // Not kidding, no...

    return glm::normalize(p) * glm::length(normal);
}
}
