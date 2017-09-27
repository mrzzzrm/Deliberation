#include <Deliberation/Scene/ScreenSpaceEffect.h>

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/ProgramInterface.h>

namespace deliberation
{
ScreenSpaceEffect::ScreenSpaceEffect() : m_initialised(false) {}

ScreenSpaceEffect::ScreenSpaceEffect(
    const std::vector<std::string> & shaders,
    const std::string &              name)
{
    m_program = GetGlobal<DrawContext>()->createProgram(shaders);

    init( name);
}

ScreenSpaceEffect::ScreenSpaceEffect(Program & program, const std::string & name)
    : m_program(program)
{
    init(name);
}

Draw & ScreenSpaceEffect::draw()
{
    Assert(m_initialised);
    return m_draw;
}

const Draw & ScreenSpaceEffect::draw() const
{
    Assert(m_initialised);
    return m_draw;
}

void ScreenSpaceEffect::render()
{
    Assert(m_initialised);
    m_draw.render();
}

void ScreenSpaceEffect::init(const std::string & name)
{
    std::vector<glm::vec2> vertices({
        {-1.0f, -1.0f},
        {1.0f, -1.0f},
        {-1.0f, 1.0f},
        {1.0f, 1.0f},
    });

    auto layout = DataLayout("Position", Type_Vec2);
    m_vertexBuffer = GetGlobal<DrawContext>()->createBuffer(layout);
    m_vertexBuffer.upload(vertices);

    m_draw = GetGlobal<DrawContext>()->createDraw(
        m_program,
        DrawPrimitive::TriangleStrip,
        name.empty() ? "ScreenSpaceEffect" : name);
    m_draw.addVertexBuffer(m_vertexBuffer);
    m_draw.state().setDepthState(DepthState::disabledRW());

    if (m_program.interface().attribute("UV"))
    {
        LayoutedBlob uvs({"UV", Type_Vec2}, 4);
        uvs.assign<glm::vec2>(
            "UV", {{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}});
        m_draw.addVertices(uvs);
    }

    m_initialised = true;
}
}
