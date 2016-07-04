#include <iostream>
#include <chrono>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <SDL2/SDL.h>

#include <Deliberation/Deliberation.h>
#include <Deliberation/Draw/GL/GLStateManager.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/BufferUpload.h>
#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Clear.h>
//#include <Deliberation/Draw.h>
//#include <Deliberation/Program.h>

#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

int main
(
    int argc,
    char * argv[]
)
{
    std::cout << "---- BasicTriangleExample ----" << std::endl;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * displayWindow;
    SDL_Renderer * displayRenderer;
    SDL_RendererInfo displayRendererInfo;

    SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);

    SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);

    if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        return 1;
    }

    SDL_GLContext glcontext = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, glcontext);

    glbinding::Binding::initialize();

    std::cout << std::endl
        << "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
        << "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
        << "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl;

    deliberation::init();

    deliberation::Context context(1600, 900);

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
    deliberation::Program program = context.createProgram({deliberation::dataPath("Data/BasicTriangleTest.vert"),
                                                           deliberation::dataPath("Data/BasicTriangleTest.frag")});

    deliberation::Draw draw = context.createDraw(program, gl::GL_TRIANGLES);

    draw.addVertexBuffer(buffer);
    draw.state().setDepthState(deliberation::DepthState(false, false));
    draw.state().setCullState(deliberation::CullState::disabled());

    Assert(draw.isComplete(), draw.toString());

    auto transform = draw.uniform("T");

    auto begin = std::chrono::system_clock::now();

    deliberation::Clear clear = context.createClear();
    clear.setColor({0.2, 0.2f, 0.2f, 0.0f});

    bool running = true;

    /* Loop until the user closes the window */
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

        auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::system_clock::now() - begin);

        draw.uniform("BaseColor").set(glm::vec3(std::sin(seconds.count()), std::cos(seconds.count()), 1.0f));

        transform.set(glm::rotate(glm::pi<float>()/2.0f * seconds.count(), glm::vec3(0, 0, -1)));

        clear.schedule();

        draw.schedule();

        SDL_RenderPresent(displayRenderer);
    }

    deliberation::shutdown();

    SDL_Quit();
    return 0;
}

