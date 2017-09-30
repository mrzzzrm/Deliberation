#pragma once

#include <string>

#include <SDL.h>

#include <Deliberation/Core/UpdateFrame.h>
#include <Deliberation/Core/FpsCounter.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Platform/InputManager.h>
#include <Deliberation/Platform/InputListener.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class AppRuntime;

class App final
{
public:
    App() = default;

    const std::string & prefix() const { return m_prefix; }
    bool gameplayPaused() const { return m_gameplayPaused; }

    void setGameplayPaused(bool paused) { m_gameplayPaused = paused; }

    float fps() const;

    template<typename RuntimeType>
    int run(const std::string & name, int argc, char ** argv);

    void quit(int returnCode = 0);

private:
    void init();
    void shutdown();
    void handleWindowEvent(const SDL_Event & event);

private:
    bool        m_running = false;
    bool        m_gameplayPaused = false;
    int         m_returnCode = 0;
    std::string m_prefix;

    // Only valid during run()
    std::unique_ptr<AppRuntime> m_runtime;

    FpsCounter m_fpsCounter;

    SDL_Window *     m_displayWindow;
    SDL_Renderer *   m_displayRenderer;
    SDL_RendererInfo m_displayRendererInfo;
    SDL_GLContext    m_glContext;

    int m_displayWidth = 1600;
    int m_displayHeight = 900;
};
}

#include <Deliberation/Platform/App.inl>