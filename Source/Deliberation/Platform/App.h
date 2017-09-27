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
    static App & get();

public:
    InputManager &       inputManager();
    const InputManager & inputManager() const;

    bool gameplayPaused() const { return m_gameplayPaused; }

    const std::shared_ptr<AppRuntime> & runtime() { return m_runtime; }

    void setGameplayPaused(bool paused) { m_gameplayPaused = paused; }

    float fps() const;

    int run(const std::shared_ptr<AppRuntime> & runtime, int argc, char ** argv);

    void quit(int returnCode = 0);

private:
    App() = default;

    void init();
    void handleWindowEvent(const SDL_Event & event);

private:
    std::shared_ptr<AppRuntime> m_runtime;
    bool        m_running = false;
    bool        m_gameplayPaused = false;
    bool        m_initialized = false;
    int         m_returnCode = 0;

    FpsCounter m_fpsCounter;

    SDL_Window *     m_displayWindow;
    SDL_Renderer *   m_displayRenderer;
    SDL_RendererInfo m_displayRendererInfo;
    SDL_GLContext    m_glContext;

    Optional<InputManager> m_inputManager;

    int m_displayWidth = 1600;
    int m_displayHeight = 900;
};
}
