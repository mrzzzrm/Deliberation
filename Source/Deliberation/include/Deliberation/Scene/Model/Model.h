#pragma once

#include <Deliberation/Draw/Buffer.h>

namespace deliberation
{

class Model final
{
public:
    Buffer vertexBuffer;

    bool hasIndices = false;
    Buffer indexBuffer;
};

}