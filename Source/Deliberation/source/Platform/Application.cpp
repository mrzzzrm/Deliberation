#include <Deliberation/Platform/Application.h>

#include <iostream>

#include <SDL_image.h>

#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/MainLoop.h>

namespace deliberation
{

Application::Application(const std::string & name):
    m_name(name),
    m_running(false),
    m_returnCode(0)
{
}

Application::~Application() = default;

InputAdapterBase & Application::inputAdapter()
{
    return m_inputAdapter;
}

const InputAdapterBase & Application::inputAdapter() const
{
    return m_inputAdapter;
}

Context & Application::context()
{
    return m_context.get();
}

const Context & Application::context() const
{
    return m_context.get();
}

int Application::run()
{
    m_running = true;

    // Init SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        m_returnCode = -1;
    }

    // Init SDL_image
    {
        int flags = IMG_INIT_JPG|IMG_INIT_PNG;
        int sucess = IMG_Init(flags);
        if (sucess & flags != flags)
        {
            std::cerr << "IMG_Init: Failed to init required jpg and png support!" << std::endl;
            std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
            m_returnCode = -1;
        }
    }

    SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_OPENGL, &m_displayWindow, &m_displayRenderer);
    if (!m_displayWindow || !m_displayRenderer)
    {
        SDL_Quit();
        m_returnCode =  -1;
    }

    SDL_GetRendererInfo(m_displayRenderer, &m_displayRendererInfo);

    if ((m_displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (m_displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        return 1;
    }

    m_glContext = SDL_GL_CreateContext(m_displayWindow);
    SDL_GL_MakeCurrent(m_displayWindow, m_glContext);

    // Init glbinding
    glbinding::Binding::initialize();

    std::cout << std::endl
        << "OpenGL Version:  " << glbinding::ContextInfo::version() << std::endl
        << "OpenGL Vendor:   " << glbinding::ContextInfo::vendor() << std::endl
        << "OpenGL Renderer: " << glbinding::ContextInfo::renderer() << std::endl;

    deliberation::init();
    deliberation::EnableGLErrorChecks();

    m_context.reset(1600, 900);

    onStartup();

    MainLoop().run([this](float seconds)
    {
        onFrame(seconds);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEMOTION:
                case SDL_MOUSEWHEEL:
                    m_inputAdapter.onSDLInputEvent(event);
                    break;

                case SDL_QUIT:
                    quit(0);
                    break;
            }
        }

        SDL_RenderPresent(m_displayRenderer);

        return m_running;
    });

    onShutdown();

    return m_returnCode;
}

void Application::quit(int returnCode)
{
    m_running = false;
    m_returnCode = returnCode;
}

void Application::onStartup()
{

}

void Application::onShutdown()
{

}

void Application::onFrame(float seconds)
{

}

}

