#include <Deliberation/Platform/Application.h>

#include <iostream>

#include <imgui.h>

#include <cxxopts.hpp>

#include <SDL_image.h>

#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <Deliberation/Core/MainLoop.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Platform/InputLayerWrapper.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
Application::Application(
    const std::string & name, const std::string & prefixPath)
    : m_name(name), m_prefixPath(prefixPath), m_running(false), m_returnCode(0)
{
}

Application::~Application() = default;

Input & Application::input()
{
    Assert(m_input.engaged(), "Input not yet setup");
    return *m_input;
}

const Input & Application::input() const
{
    Assert(m_input.engaged(), "Input not yet setup");
    return *m_input;
}

DrawContext & Application::drawContext() { return m_drawContext.get(); }

const DrawContext & Application::drawContext() const
{
    return m_drawContext.get();
}

float Application::fps() const { return m_fpsCounter.fps(); }

int Application::run(int argc, char ** argv)
{
    /**
     * Parse command line
     */
    std::string cmdPrefix;

    cxxopts::Options options(m_name, "");
    options.add_options()(
        "prefix",
        "Deliberation install prefix",
        cxxopts::value<std::string>(cmdPrefix));
    options.parse(argc, argv);

    if (!cmdPrefix.empty())
    {
        std::cout << "Prefix override: '" << cmdPrefix << "'" << std::endl;
        m_prefixPath = cmdPrefix;
    }

    /**
     *
     */
    init();

    m_running = true;

    onStartup();

    if (deliberation::GLLoggingEnabled())
        std::cout << "--- Frame ---" << std::endl;

    MainLoop().run([this](float seconds) {
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
            case SDL_MOUSEWHEEL: m_input->onSDLInputEvent(event); break;

            case SDL_WINDOWEVENT: handleWindowEvent(event); break;

            case SDL_QUIT: quit(0); break;
            }
        }
        m_input->onFrameBegin();

        SDL_GL_MakeCurrent(m_displayWindow, m_glContext);

        onFrame(seconds);

        SDL_GL_SwapWindow(m_displayWindow);

        m_fpsCounter.onFrame();

        if (deliberation::GLLoggingEnabled())
            std::cout << "--- Frame ---" << std::endl;

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

void Application::onStartup() {}

void Application::onShutdown() {}

void Application::onFrame(float seconds) {}

void Application::init()
{
    m_initialized = false;

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        m_returnCode = -1;
    }

    // Init SDL_image
    {
        int flags = IMG_INIT_JPG | IMG_INIT_PNG;
        int sucess = IMG_Init(flags);
        if ((sucess & flags) != flags)
        {
            std::cerr
                << "IMG_Init: Failed to init required jpg and png support!"
                << std::endl;
            std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
            m_returnCode = -1;
            return;
        }
    }

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

    m_displayWindow = SDL_CreateWindow(
        "Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_displayWidth,
        m_displayHeight,
        SDL_WINDOW_OPENGL);
    if (!m_displayWindow)
    {
        SDL_Quit();
        m_returnCode = -1;
        return;
    }

    m_glContext = SDL_GL_CreateContext(m_displayWindow);
    SDL_GL_MakeCurrent(m_displayWindow, m_glContext);

    glm::ivec4 drawContextColorSize;
    SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &drawContextColorSize.x);
    SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &drawContextColorSize.y);
    SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &drawContextColorSize.z);
    SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &drawContextColorSize.w);

    // Init glbinding
    glbinding::Binding::initialize();

    auto versionString = glbinding::ContextInfo::version().toString();
    auto vendorString = glbinding::ContextInfo::vendor();
    auto rendererString = glbinding::ContextInfo::renderer();

    std::cout << std::endl
              << "OpenGL Version:  " << versionString << std::endl
              << "OpenGL Vendor:   " << vendorString << std::endl
              << "OpenGL Renderer: " << rendererString << std::endl
              << "DrawContext RGBA-bits: " << drawContextColorSize << std::endl;

    deliberation::init();
    deliberation::setPrefixPath(m_prefixPath);
    deliberation::EnableGLErrorChecks();

    std::cout << "Deliberation initialized with prefix '"
              << deliberation::prefixPath() << "'" << std::endl;

    m_drawContext.reset(m_displayWidth, m_displayHeight);

    /**
     * Init input
     */
    m_input.reset();
    m_input->addLayer(std::make_shared<InputLayerWrapper>(
        *this, std::numeric_limits<i32>::min()));

    m_initialized = true;
}

void Application::handleWindowEvent(const SDL_Event & event) {}
}
