#pragma once

#include <string>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API Morton2D final
{
public:
    static unsigned int NumKeysAtDepth(unsigned int depth);

public:
    inline Morton2D(uint32_t key = 0);
    inline Morton2D(const glm::uvec2 & cell);

    inline uint32_t toIndex() const;
    inline glm::uvec2 toVec() const;

    std::string toString() const;

private:
    uint32_t m_key;
};

class DELIBERATION_API Morton3D final
{
public:
    static unsigned int NumKeysAtDepth(unsigned int depth);

public:
    inline Morton3D(uint32_t key = 0);
    inline Morton3D(const glm::uvec3 & cell);

    inline uint32_t toIndex() const;
    inline glm::uvec3 toVec() const;

    std::string toString() const;

private:
    uint32_t m_key;
};

}

#include <Deliberation/Core/Math/Morton.inl>
