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

using namespace deliberation;

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
                                                {"Color", deliberation::Type_U8Vec4}});

        auto vertices = deliberation::LayoutedBlob(layout, 4);

        vertices.assign<glm::vec2>("Position", {{-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}});
       // vertices.assign<glm::vec3>("Color", {{1, 0, 0}, {1, 0, 0}, {1, 0, 0}});
        vertices.assign<glm::u8vec4>("Color", {{255, 0, 0, 255}, {255, 0, 0, 0}, {255, 0, 0, 0}, {255, 0, 0, 255}});

        auto indicesBlob = deliberation::LayoutedBlob(DataLayout("Index", Type_U32), 6);
        indicesBlob.assign<u32>("Index", {0, 1, 2, 0, 2, 3});

        auto program = context().createProgram({deliberation::DeliberationDataPath("Data/BasicTriangleTest.vert"),
                                                deliberation::DeliberationDataPath("Data/BasicTriangleTest.frag")});

        m_draw = context().createDraw(program, gl::GL_TRIANGLES);
        m_draw.addVertices(vertices);
        m_draw.setIndices(indicesBlob);
        m_draw.state().setDepthState(deliberation::DepthState::disabledRW());
        m_draw.state().setCullState(deliberation::CullState::disabled());
        m_draw.state().setBlendState({gl::GL_FUNC_ADD, gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA});

        Assert(m_draw.isComplete(), m_draw.toString());

        m_transform = m_draw.uniform("T");
        m_fadeUniform = m_draw.uniform("Fade");

        m_clear = context().createClear();
        m_clear.setColor({0.2, 0.2f, 0.2f, 0.0f});
    }

    void onFrame(float seconds) override
    {
        m_fade -= seconds * 0.1f;
        m_fade = std::max(m_fade, 0.0f);

        std::cout << m_fade << " " << seconds << std::endl;

        m_transform.set(glm::mat4(1.0f));
        m_fadeUniform.set(m_fade);

        m_clear.render();
        m_draw.schedule();

        m_accumSeconds += seconds;
    }

private:
    deliberation::Draw      m_draw;
    deliberation::Uniform   m_transform;
    deliberation::Uniform   m_fadeUniform;
    deliberation::Clear     m_clear;
    float                   m_accumSeconds = 0.0f;
    float                   m_fade = 1.0f;
};

int main
(
    int argc,
    char * argv[]
)
{
    return BasicTriangleExample().run(argc, argv);
}

