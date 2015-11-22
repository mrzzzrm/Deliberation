#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

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

    glbinding::setCallbackMask(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue);
    glbinding::setAfterCallback([](const glbinding::FunctionCall & call)
    {
        glbinding::setCallbackMask(glbinding::CallbackMask::None);
//        gl::GLenum error;
//        while((error = gl::glGetError()) != gl::GL_NO_ERROR)
//        {
//            std::cout << "GL Error: " << error << std::endl;
//            std::cout << "  after calling ";
            std::cout << call.function->name() << "(";
            for (unsigned i = 0; i < call.parameters.size(); ++i)
            {
                std::cout << call.parameters[i]->asString();
                if (i < call.parameters.size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << ")";
            std::cout << std::endl;
//            assert(false);
//        }
        glbinding::setCallbackMask(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue);
    });

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
    draw.addVertexBuffer(buffer);
    draw.state().setDepthState(deliberation::DepthState(false, false));
    draw.state().setCullState(deliberation::CullState::disabled());
    Assert(draw.isComplete(), draw.toString());

//
//    deliberation::Clear clear = context.createClear();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        gl::glClearColor(0.2, 0.2f, 0.2f, 0.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

//        clear.schedule();
        draw.schedule();
//       break;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

