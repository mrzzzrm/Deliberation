#include <Deliberation/Scene/PostprocessingEffect.h>

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/ProgramInterface.h>

namespace deliberation
{

PostprocessingEffect::PostprocessingEffect():
    m_initialised(false)
{

}

PostprocessingEffect::PostprocessingEffect(Context & context,
                                           const std::vector<std::string> & shaders,
                                           const std::string & name):
    m_initialised(true)
{
    m_program = context.createProgram(shaders);

    std::cout << m_program.interface().toString() << std::endl;

    std::vector<glm::vec2> vertices({
        {-1.0f, -1.0f},
        { 1.0f, -1.0f},
        {-1.0f,  1.0f},
        { 1.0f,  1.0f},
    });

    auto layout = DataLayout("Position", Type_Vec2);
    m_vertexBuffer = context.createBuffer(layout);
    m_vertexBuffer.scheduleUpload(vertices);

    m_draw = context.createDraw(m_program, gl::GL_TRIANGLE_STRIP, name.empty() ? "PostprocessingEffect" : name);
    m_draw.addVertexBuffer(m_vertexBuffer);
    m_draw.state().setDepthState(DepthState::disabledRW());
}

Draw & PostprocessingEffect::draw()
{
    Assert(m_initialised, "");
    return m_draw;
}

const Draw & PostprocessingEffect::draw() const
{
    Assert(m_initialised, "");
    return m_draw;
}

void PostprocessingEffect::schedule()
{
    Assert(m_initialised, "");
    m_draw.schedule();
}

}

