#pragma once

#include <Deliberation/Core/Math/AABB.h>

#include <Deliberation/Draw/Buffer.h>

namespace deliberation
{
class Model final
{
  public:
    AABB bounds;

    Buffer vertexBuffer;

    bool   hasIndices = false;
    Buffer indexBuffer;
};
}