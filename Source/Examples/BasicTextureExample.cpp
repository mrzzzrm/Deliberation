#include <iostream>
#include <chrono>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Deliberation/Deliberation.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/BufferUpload.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Clear.h>
#include <Deliberation/Draw/TextureLoader.h>
#include <Deliberation/Platform/Application.h>

struct Vertex
{
    glm::vec2 position;
    glm::vec2 uv;
};

class BasicTextureExample:
    public deliberation::Application {
public:
    BasicTextureExample() :
        Application("BasicTextureExample") {

    }

    virtual void onStartup() override
    {
        auto layout = context().createBufferLayout<Vertex>({
                                                             {"Position", &Vertex::position},
                                                             {"UV", &Vertex::uv},
                                                           });

        auto vbuffer = context().createBuffer(layout);
        auto vertices = std::vector<Vertex>({
                                                {{-0.5f, 0.0f}, {0.0f, 0.0f}},
                                                {{0.5f, 0.0f}, {1.0f, 0.0f}},
                                                {{0.0f, 0.5f}, {1.0f, 1.0f}},
                                                {{-1.0f, 0.5f}, {0.0f, 1.0f}}
                                            });
        vbuffer.createUpload(vertices).schedule();

        auto ibuffer = context().createIndexBuffer8();
        auto indices = std::vector<gl::GLbyte>({
                                                   0, 1, 2,
                                                   0, 2, 3
                                               });
        ibuffer.createUpload(indices).schedule();

        auto program = context().createProgram({deliberation::dataPath("Data/BasicTextureTest.vert"),
                                                deliberation::dataPath("Data/BasicTextureTest.frag")});

        m_texture = context().createTexture(deliberation::TextureLoader(deliberation::dataPath("Data/Examples/testimage.png")).load());

        m_draw = context().createDraw(program, gl::GL_TRIANGLES);
        m_draw.sampler("Texture").setTexture(m_texture);
        m_draw.addVertexBuffer(vbuffer);
        m_draw.setIndexBuffer(ibuffer);
        m_draw.state().setDepthState(deliberation::DepthState(false, false));
        m_draw.state().setCullState(deliberation::CullState::disabled());
        Assert(m_draw.isComplete(), m_draw.toString());

        m_transform = m_draw.uniform("T");

        m_binaries.push_back(deliberation::TextureLoader(deliberation::dataPath("Data/Examples/0.png")).load());
        m_binaries.push_back(deliberation::TextureLoader(deliberation::dataPath("Data/Examples/1.png")).load());
        m_binaries.push_back(deliberation::TextureLoader(deliberation::dataPath("Data/Examples/2.png")).load());

        m_morphingTexture = context().createTexture(m_binaries.front());
        m_lastTextureMorph = std::chrono::system_clock::now();

        m_clear = context().createClear();
        m_clear.setColor({0.2, 0.2f, 0.2f, 0.0f});
    }

    virtual void onFrame(float seconds) override
    {
        m_clear.schedule();

        m_transform.set(glm::rotate(glm::pi<float>()/2.0f * m_accumSeconds, glm::vec3(0, 0, -1)));
        m_draw.sampler("Texture").setTexture(m_texture);
        m_draw.schedule();

        if (std::chrono::system_clock::now() - m_lastTextureMorph > std::chrono::seconds(1))
        {
            m_currentBinaryIndex++;
            m_currentBinaryIndex %= m_binaries.size();
            m_morphingTexture.createUpload(m_binaries[m_currentBinaryIndex]).schedule();
            m_lastTextureMorph = std::chrono::system_clock::now();
        }
        m_transform.set(glm::scale(glm::mat4(1.0f), glm::vec3(0.8f)));
        m_draw.sampler("Texture").setTexture(m_morphingTexture);
        m_draw.schedule();

        m_accumSeconds += seconds;
    }

private:
    deliberation::Draw      m_draw;
    deliberation::Uniform   m_transform;
    deliberation::Clear     m_clear;
    deliberation::Texture   m_texture;
    deliberation::Texture   m_morphingTexture;

    float                   m_accumSeconds = 0.0f;

    std::chrono::system_clock::time_point       m_lastTextureMorph;
    int                                         m_currentBinaryIndex = 0;
    std::vector<deliberation::TextureBinary>    m_binaries;
};

int main
(
    int argc,
    char * argv[]
)
{
    return BasicTextureExample().run();
}

