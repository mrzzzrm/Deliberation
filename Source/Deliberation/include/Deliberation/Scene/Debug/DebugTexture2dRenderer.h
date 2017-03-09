#pragma once

#include <Deliberation/Draw/Draw.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class Context;
class Texture;

class DELIBERATION_API DebugTexture2dRenderer final
{
public:
    DebugTexture2dRenderer(Context & context, const Texture & texture);

    void render();

private:
    Draw m_draw;
};

}