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

template<typename RuntimeType>
int App::run(const std::string & name, int argc, char ** argv)
{
    /**
     * Parse command line
     */
    std::string cmdPrefix;

    cxxopts::Options options(name, "");
    options.add_options()(
        "prefix",
        "Deliberation install prefix",
        cxxopts::value<std::string>(cmdPrefix));
    options.parse(argc, argv);

    if (!cmdPrefix.empty())
    {
        Log->info("Prefix override: '{}'", cmdPrefix);
        m_prefix = cmdPrefix;
    }
    else
    {
        m_prefix = ".";
    }

    /**
     *
     */
    init();

    m_runtime = std::make_unique<RuntimeType>(); // AFTER init()
    m_runtime->onStartup();

    m_running = true;

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
                case SDL_MOUSEWHEEL: GetGlobal<InputManager>()->onSDLInputEvent(event); break;

                case SDL_WINDOWEVENT: handleWindowEvent(event); break;

                case SDL_QUIT: quit(0); break;
            }
        }
        GetGlobal<InputManager>()->onFrameBegin();

        SDL_GL_MakeCurrent(m_displayWindow, m_glContext);

        m_runtime->onFrame(micros);

        SDL_GL_SwapWindow(m_displayWindow);

        m_fpsCounter.onFrame();

        if (deliberation::GLLoggingEnabled()) Log->info("--- Frame ---");

        frameCounter++;

        return m_running;
    });

    m_runtime->onShutdown();
    m_runtime.reset();

    shutdown();

    return m_returnCode;
}

}