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
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/BufferUpload.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Clear.h>
//#include <Deliberation/Draw.h>
//#include <Deliberation/Program.h>

int main
(
    int argc,
    char * argv[]
)
{
    std::cout << "---- BasicTextureExample ----" << std::endl;

    GLFWwindow * window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "BasicTextureExample", NULL, NULL);
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
        {{-0.5f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.0f}, {1.0f, 0.0f}},
        {{0.0f, 0.5f}, {1.0f, 1.0f}},
        {{-1.0f, 0.5f}, {0.0f, 1.0f}}
    });
    vbuffer.createUpload(vertices).schedule();

    auto ibuffer = context.createIndexBuffer8();
    auto indices = std::vector<gl::GLbyte>({
        0, 1, 2,
        0, 2, 3
    });
    ibuffer.createUpload(indices).schedule();

    std::cout << "Buffer has " << ibuffer.count() << " indices" << std::endl;

    auto program = context.createProgram({deliberation::dataPath("Data/BasicTextureTest.vert"), deliberation::dataPath("Data/BasicTextureTest.frag")});

    auto texture = context.createTexture(deliberation::dataPath("Data/testimage.png"));
    std::cout << "Texture resolution: " << texture.width() << "x" << texture.height() << std::endl;

    auto draw = context.createDraw(program, gl::GL_TRIANGLES);
    draw.sampler("Texture").setTexture(texture);
    draw.addVertexBuffer(vbuffer);
    draw.setIndexBuffer(ibuffer);
    draw.state().setDepthState(deliberation::DepthState(false, false));
    draw.state().setCullState(deliberation::CullState::disabled());

    Assert(draw.isComplete(), draw.toString());

    auto transform = draw.uniform("T");

    auto begin = std::chrono::system_clock::now();

    auto clear = context.createClear();
    clear.setColor({0.2, 0.2f, 0.2f, 0.0f});

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::system_clock::now() - begin);

        transform.set(glm::rotate(glm::pi<float>()/2.0f * seconds.count(), glm::vec3(0, 0, -1)));

        clear.schedule();

//        clear.schedule();
        draw.schedule();
//       break;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    deliberation::shutdown();

    glfwTerminate();
    return 0;
}

