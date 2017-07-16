#pragma once

#include <Deliberation/Draw/Draw.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class DrawContext;
class Texture;

class DebugTexture2dRenderer final
{
public:
    DebugTexture2dRenderer(DrawContext & drawContext, const Texture & texture);

    void render();

private:
    Draw m_draw;
};
}