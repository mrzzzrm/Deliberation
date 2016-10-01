#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/Span.h>

namespace deliberation
{

void DELIBERATION_API PolygonClipping2D(const Span<glm::vec2> & subject,
                                        const Span<glm::vec2> & clipping,
                                        std::vector<glm::vec2> & result);

}