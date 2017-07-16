#pragma once

#include <string>
#include <vector>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>

namespace deliberation
{
class DrawContext;

class ScreenSpaceEffect final
{
public:
    ScreenSpaceEffect();
    ScreenSpaceEffect(
        DrawContext &                    drawContext,
        const std::vector<std::string> & shaders,
        const std::string &              name = std::string());
    ScreenSpaceEffect(
        DrawContext &       drawContext,
        Program &           program,
        const std::string & name = std::string());

    Draw &       draw();
    const Draw & draw() const;

    DrawContext & drawContext() const
    {
        Assert(m_drawContext, "");
        return *m_drawContext;
    }

    void render();

private:
    void init(DrawContext & drawContext, const std::string & name);

private:
    DrawContext * m_drawContext = nullptr;
    bool          m_initialised;
    Draw          m_draw;
    Program       m_program;
    Buffer        m_vertexBuffer;
};
}
