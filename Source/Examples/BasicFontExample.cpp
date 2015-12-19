#include <iostream>
#include <chrono>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Viewport.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/BufferUpload.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Clear.h>

#include <Deliberation/Font/Font.h>
#include <Deliberation/Font/Label.h>
#include <Deliberation/Font/LabelRenderer.h>

GLFWwindow * window;

void run
(
)
{
    deliberation::Context context;

    struct Vertex
    {
        glm::vec2 position;
        glm::vec2 uv;
    };

    auto layout = context.createBufferLayout<Vertex>({
        {"Position", &Vertex::position},
        {"UV", &Vertex::uv},
    });
    auto vbuffer = context.createBuffer(layout);
    auto vertices = std::vector<Vertex>({
        {{-0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f}, {1.0f, 0.0f}}
    });
    vbuffer.createUpload(vertices).schedule();

    auto ibuffer = context.createIndexBuffer8();
    auto indices = std::vector<gl::GLbyte>({
        0, 1, 2,
        0, 2, 3
    });
    ibuffer.createUpload(indices).schedule();

    std::cout << "Buffer has " << ibuffer.count() << " indices" << std::endl;

    auto program = context.createProgram({deliberation::dataPath("Data/BasicFontTest.vert"), deliberation::dataPath("Data/BasicFontTest.frag")});

    deliberation::Font font(context, deliberation::dataPath("Data/Xolonium.ttf"));

    auto texture = font.render("Hello Font World", 64, glm::vec4(0.2f, 0.4f, 0.6f, 0.8f));
    std::cout << "Texture resolution: " << texture.width() << "x" << texture.height() << std::endl;

    deliberation::LabelRenderer labelRenderer;
    labelRenderer = deliberation::LabelRenderer(context);
//    deliberation::LabelRenderer labelRenderer(context);
    deliberation::Label label(font);
    label.setText("Bonjour!");
    label.setPosition({-1.0f, 0.0f});
    label.setCenter({-1.0f, 0.0f});
    label.setColor({1.0f, 1.0f, 0.0f});

    auto draw = context.createDraw(program, gl::GL_TRIANGLES);
    draw.sampler("Texture").setTexture(texture);
    draw.addVertexBuffer(vbuffer);
    draw.setIndexBuffer(ibuffer);
    draw.state().setDepthState(deliberation::DepthState(false, false));
    draw.state().setCullState(deliberation::CullState::disabled());

    Assert(draw.isComplete(), draw.toString());

    auto clear = context.createClear();
    clear.setColor({0.2, 0.2f, 0.2f, 0.0f});

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        clear.schedule();
        draw.schedule();

        labelRenderer.render(label, deliberation::Viewport(0, 0, 640, 480));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

int main
(
    int argc,
    char * argv[]
)
{
    std::cout << "---- BasicFontTest ----" << std::endl;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glbinding::Binding::initialize();

    deliberation::init();
    deliberation::setPrefixPath("..");

    run();

    deliberation::shutdown();

    glfwTerminate();
    return 0;
}

