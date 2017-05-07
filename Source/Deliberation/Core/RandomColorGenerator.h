#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class RandomColorGenerator final
{
  public:
    RandomColorGenerator();

    glm::vec3 generate();

  private:
    glm::vec3 hsvToRGB(const glm::vec3 & hsv) const;

  private:
    float m_val;
};
}