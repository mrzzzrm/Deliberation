#include <Deliberation/Platform/Application.h>

#include <iostream>

#include <imgui.h>

#include <cxxopts.hpp>

#include <SDL_image.h>

#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <Deliberation/Core/Log.h>
#include <Deliberation/Core/MainLoop.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Platform/ApplicationRuntime.h>
#include <Deliberation/Platform/InputLayerWrapper.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

Application & Application::instance()
{
    static Application application;
    return application;
}

InputManager & Application::inputManager()
{
    AssertM(m_inputManager.engaged(), "InputManager not yet setup");
    return *m_inputManager;
}

const InputManager & Application::inputManager() const
{
    AssertM(m_inputManager.engaged(), "InputManager not yet setup");
    return *m_inputManager;
}

DrawContext & Application::drawContext() { return m_drawContext.get(); }

const DrawContext & Application::drawContext() const
{
    return m_drawContext.get();
}

float Application::fps() const { return m_fpsCounter.fps(); }

int Application::run(const std::shared_ptr<ApplicationRuntime> & runtime, int argc, char ** argv)
{
    deliberation::init();

    m_runtime = runtime;

    /**
     * Parse command line
     */
    std::string cmdPrefix;

    cxxopts::Options options(m_runtime->name(), "");
    options.add_options()(
        "prefix",
        "Deliberation install prefix",
        cxxopts::value<std::string>(cmdPrefix));
    options.parse(argc, argv);

    if (!cmdPrefix.empty())
    {
        Log->info("Prefix override: '{}'", cmdPrefix);
        m_runtime->setPrefix(cmdPrefix);
    }

    /**
     *
     */
    init();

    m_running = true;

    m_runtime->onStartup();

    if (deliberation::GLLoggingEnabled()) Log->info("--- Frame ---");

    u32 frameCounter = 0;

    MainLoop().run([&](DurationMicros micros) {
        LogSetFrameIndex(frameCounter);

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
            case SDL_MOUSEWHEEL: m_inputManager->onSDLInputEvent(event); break;

            case SDL_WINDOWEVENT: handleWindowEvent(event); break;

            case SDL_QUIT: quit(0); break;
            }
        }
        m_inputManager->onFrameBegin();

        SDL_GL_MakeCurrent(m_displayWindow, m_glContext);

        m_runtime->onFrame(micros);

        SDL_GL_SwapWindow(m_displayWindow);

        m_fpsCounter.onFrame();

        if (deliberation::GLLoggingEnabled()) Log->info("--- Frame ---");

        frameCounter++;

        return m_running;
    });

    m_runtime->onShutdown();

    deliberation::shutdown();

    return m_returnCode;
}

void Application::quit(int returnCode)
{
    m_running = false;
    m_returnCode = returnCode;
}

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
        AssertM((sucess & flags) == flags, "IMG_Init: Failed to init required jpg and png support! " + std::string(IMG_GetError()));
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

    Log->info("OpenGL Version:        {}", versionString);
    Log->info("OpenGL Vendor:         {}", vendorString);
    Log->info("OpenGL Renderer:       {}", rendererString);
    Log->info("DrawContext RGBA-bits: {}", drawContextColorSize);

    deliberation::setPrefixPath(m_runtime->prefix());
    deliberation::EnableGLErrorChecks();

    Log->info("Prefix:                {}", deliberation::prefixPath());

    m_drawContext.reset(m_displayWidth, m_displayHeight);

    /**
     * Init input
     */
    m_inputManager.reset();

    m_initialized = true;
}

void Application::handleWindowEvent(const SDL_Event & event) {}
}
