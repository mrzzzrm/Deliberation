#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include <glm/glm.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/BufferUpload.h>
#include <Deliberation/Draw/Context.h>
//#include <Deliberation/Clear.h>
//#include <Deliberation/Draw.h>
//#include <Deliberation/Program.h>

int main
(
    int argc,
    char * argv[]
)
{
    GLFWwindow* window;

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

    globjects::init();

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
        {{0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    });
    context.createBufferUpload(buffer, vertices).schedule();
    deliberation::Program program = context.createProgram({"Data/BasicTriangleTest.vert", "Data/BasicTriangleTest.frag"});

    deliberation::Draw draw = context.createDraw(program, gl::GL_TRIANGLES);
//    draw.addVertexBuffer(buffer);
//
//    deliberation::Clear clear = context.createClear();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
//        clear.schedule();
//        draw.schedule();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

