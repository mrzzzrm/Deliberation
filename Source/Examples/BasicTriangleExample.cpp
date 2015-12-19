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
    std::cout << "---- BasicTriangleExample ----" << std::endl;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "BasicTriangleExample", NULL, NULL);
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
        glm::vec3 color;
    };

    deliberation::BufferLayout layout = context.createBufferLayout<Vertex>({
        {"Position", &Vertex::position},
        {"Color", &Vertex::color},
    });

    std::cout << layout.toString() << std::endl;
    deliberation::Buffer buffer = context.createBuffer(layout);

    auto vertices = std::vector<Vertex>({
        {{-0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    });
    buffer.createUpload(vertices).schedule();
    deliberation::Program program = context.createProgram({deliberation::dataPath("Data/BasicTriangleTest.vert"), deliberation::dataPath("Data/BasicTriangleTest.frag")});

    deliberation::Draw draw = context.createDraw(program, gl::GL_TRIANGLES);

    draw.addVertexBuffer(buffer);
    draw.state().setDepthState(deliberation::DepthState(false, false));
    draw.state().setCullState(deliberation::CullState::disabled());

    Assert(draw.isComplete(), draw.toString());

    auto transform = draw.uniform("T");

    auto begin = std::chrono::system_clock::now();

    deliberation::Clear clear = context.createClear();
    clear.setColor({0.2, 0.2f, 0.2f, 0.0f});

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::system_clock::now() - begin);

        draw.uniform("BaseColor").set(glm::vec3(std::sin(seconds.count()), std::cos(seconds.count()), 1.0f));

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

