#include <iostream>
#include <chrono>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Viewport.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/BufferUpload.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Clear.h>

#include <Deliberation/Font/Font.h>
#include <Deliberation/Font/Label.h>
#include <Deliberation/Font/LabelRenderer.h>
#include <Deliberation/Platform/Application.h>

struct Vertex
{
    glm::vec2 position;
    glm::vec2 uv;
};

class BasicFontExample:
    public deliberation::Application
{
public:
    BasicFontExample():
        deliberation::Application("BasicFontExample")
    {

    }

    virtual void onStartup() override
    {

        auto layout = deliberation::DataLayout::fromStructOrClass<Vertex>({
                                                             {"Position", &Vertex::position},
                                                             {"UV", &Vertex::uv},
                                                         });
        auto vbuffer = context().createBuffer(layout);
        auto vertices = std::vector<Vertex>({
                                                {{-0.5f, -0.5f}, {0.0f, 0.0f}},
                                                {{-0.5f,  0.5f}, {0.0f, 1.0f}},
                                                {{ 0.5f,  0.5f}, {1.0f, 1.0f}},
                                                {{ 0.5f, -0.5f}, {1.0f, 0.0f}}
                                            });
        vbuffer.createUpload(vertices).schedule();

        auto ibuffer = context().createIndexBuffer8();
        auto indices = std::vector<gl::GLbyte>({
                                                   0, 1, 2,
                                                   0, 2, 3
                                               });
        ibuffer.createUpload(indices).schedule();

        auto program = context().createProgram({deliberation::dataPath("Data/BasicFontTest.vert"),
                                                deliberation::dataPath("Data/BasicFontTest.frag")});

        m_font.reset(context(), deliberation::dataPath("Data/Xolonium.ttf"));

        auto texture = m_font.get().render("Hello Font World", 64, glm::vec4(0.2f, 0.4f, 0.6f, 0.8f));

        m_labelRenderer = deliberation::LabelRenderer(context());

        m_label.reset(m_font.get());
        m_label.get().setText("Bonjour!");
        m_label.get().setPosition({-1.0f, 0.0f});
        m_label.get().setCenter({-1.0f, 0.0f});
        m_label.get().setColor({1.0f, 1.0f, 0.0f});

        m_draw = context().createDraw(program, gl::GL_TRIANGLES);
        m_draw.sampler("Texture").setTexture(texture);
        m_draw.addVertexBuffer(vbuffer);
        m_draw.setIndexBuffer(ibuffer);
        m_draw.state().setDepthState(deliberation::DepthState::disabledRW());
        m_draw.state().setCullState(deliberation::CullState::disabled());

        m_clear = context().createClear();
        m_clear.setColor({0.2, 0.2f, 0.2f, 0.0f});
    }

    virtual void onFrame(float seconds) override
    {
        m_clear.schedule();
        m_draw.schedule();

        m_labelRenderer.render(m_label.get(), deliberation::Viewport(0, 0, 640, 480));
    }

private:
    deliberation::Draw                          m_draw;
    deliberation::Clear                         m_clear;
    deliberation::Optional<deliberation::Label> m_label;
    deliberation::Optional<deliberation::Font>  m_font;
    deliberation::LabelRenderer                 m_labelRenderer;
};


int main
(
    int argc,
    char * argv[]
)
{
    return BasicFontExample().run();
}

