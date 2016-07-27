#include <iostream>
#include <chrono>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <SDL2/SDL.h>

#include <Deliberation/Deliberation.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Draw/GL/GLStateManager.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Platform/Application.h>

#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>
struct Vertex
{
    glm::vec2 position;
    glm::vec3 color;
};

class BasicTriangleExample:
    public deliberation::Application
{
public:
    BasicTriangleExample():
        Application("BasicTriangleExample")
    {
    }

    virtual void onStartup() override
    {
        auto layout = deliberation::DataLayout({{"Position", deliberation::Type_Vec2},
                                                {"Color", deliberation::Type_Vec3}});

        auto vertices = deliberation::LayoutedBlob(layout, 3);

        vertices.assign<glm::vec2>("Position", {{-0.5f, 0.0f}, {0.5f, 0.0f}, {0.0f, 0.5f}});
        vertices.assign<glm::vec3>("Color", {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});

        auto program = context().createProgram({deliberation::dataPath("Data/BasicTriangleTest.vert"),
                                                deliberation::dataPath("Data/BasicTriangleTest.frag")});

        m_draw = context().createDraw(program, gl::GL_TRIANGLES);
        m_draw.addVertices(vertices.layout(), vertices.rawData());
        m_draw.state().setDepthState(deliberation::DepthState(false, false));
        m_draw.state().setCullState(deliberation::CullState::disabled());

        Assert(m_draw.isComplete(), m_draw.toString());

        m_transform = m_draw.uniform("T");

        m_clear = context().createClear();
        m_clear.setColor({0.2, 0.2f, 0.2f, 0.0f});
    }

    virtual void onFrame(float seconds)
    {
        m_draw.uniform("BaseColor").set(glm::vec3(std::sin(seconds), std::cos(seconds), 1.0f));
        m_transform.set(glm::rotate(glm::pi<float>()/2.0f * m_accumSeconds, glm::vec3(0, 0, -1)));

        m_clear.schedule();
        m_draw.schedule();

        m_accumSeconds += seconds;
    }

private:
    deliberation::Draw      m_draw;
    deliberation::Uniform   m_transform;
    deliberation::Clear     m_clear;
    float                   m_accumSeconds = 0.0f;
};

int main
(
    int argc,
    char * argv[]
)
{
    return BasicTriangleExample().run();
}

