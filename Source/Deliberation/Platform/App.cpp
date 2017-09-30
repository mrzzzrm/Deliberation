#include <Deliberation/Platform/App.h>

#include <iostream>

#include <imgui.h>

#include <cxxopts.hpp>

#include <spdlog/spdlog.h>

#include <SDL_ttf.h>
#include <SDL_image.h>

#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Log.h>
#include <Deliberation/Core/MainLoop.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Platform/AppRuntime.h>
#include <Deliberation/Platform/InputLayerWrapper.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
DELIBERATION_DEFINE_GLOBAL(App);

float App::fps() const { return m_fpsCounter.fps(); }

void App::quit(int returnCode)
{
    m_running = false;
    m_returnCode = returnCode;
}

void App::init()
{
    // Create logger
    Log = spdlog::stdout_color_mt("Main");
    LogSetOuterScope("*PreMainLoop*");

    // SDL_ttf
    {
        auto r = TTF_Init();
        if (r)
        {
            std::cout << "Failed to init SDL_ttf: '" << TTF_GetError() << "'"
                      << std::endl;
        }
    }

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

    deliberation::EnableGLErrorChecks();

    Log->info("Prefix:                {}", m_prefix);

    InitGlobal<DrawContext>();
    GetGlobal<DrawContext>()->setBackbufferResolution(m_displayWidth, m_displayHeight);

    InitGlobal<InputManager>();
}


void App::shutdown()
{
    // SDL_ttf
    if (TTF_WasInit())
    {
        TTF_Quit();
    }
}

void App::handleWindowEvent(const SDL_Event & event) {}
}
