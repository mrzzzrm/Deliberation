#include <iostream>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/TextureLoader.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/PixelFormat.h>

#include <Deliberation/Platform/Application.h>


using namespace deliberation;

class DrawSandbox:
    public Application
{
public:
    DrawSandbox():
        Application("DrawSandbox")
    {

    }

    void onStartup() override
    {
        DisableGLErrorChecks();

        m_clear = drawContext().createClear();

        const auto program = drawContext().createProgram({
            DeliberationDataPath("Data/Shaders/Rgb_Position2_InstancePosition2.vert"),
            DeliberationDataPath("Data/Shaders/Rgb.frag")
                                                     });

        m_draw = drawContext().createDraw(program);

        const auto vertexLayout = DataLayout("Position", Type_Vec2);

        auto vertices = LayoutedBlob(vertexLayout, 3);

        vertices.field<glm::vec2>("Position").assign({
            glm::vec2(-1.0f, -1.0f),
            glm::vec2(1.0f, -1.0f),
            glm::vec2(1.0f, 1.0f)
        });

        m_draw.addVertices(vertices);

        m_draw.setAttribute("Rgb", glm::vec3(1.0f, 0.0f, 0.5f));
        m_draw.state().setCullState(CullState::disabled());
        m_draw.state().setDepthState(DepthState::disabledRW());

        m_position = glm::vec2(0.2f, 0.1f);
    }

    void onFrame(float seconds) override
    {
        m_secondsAccumulator += seconds;

        m_draw.setAttribute("InstancePosition", m_position);
        m_position.x = std::abs(std::sin(m_secondsAccumulator * 0.1f));

        m_clear.render();
        m_draw.schedule();
    }

private:
    Clear       m_clear;
    Draw        m_draw;
    glm::vec2   m_position;
    float       m_secondsAccumulator = 0.0f;
};

int main(int argc, char * argv[])
{
    return DrawSandbox().run(argc, argv);
}

