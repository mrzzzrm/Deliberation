#pragma once

#include <string>

#include <SDL.h>

#include <Deliberation/Core/UpdateFrame.h>
#include <Deliberation/Core/FpsCounter.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Platform/Input.h>
#include <Deliberation/Platform/InputEventReceiver.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class Application : public InputEventReceiver
{
public:
    Application(const std::string & name, const std::string & prefixPath = ".");

    Input &       input();
    const Input & input() const;

    DrawContext &       drawContext();
    const DrawContext & drawContext() const;

    UpdateFrame & updateFrame() { return m_updateFrame; }
    const UpdateFrame & updateFrame() const { return m_updateFrame; }

    bool gameplayPaused() const { return m_gameplayPaused; }
    void setGameplayPaused(bool paused) { m_gameplayPaused = paused; }

    float fps() const;

    int run(int argc, char ** argv);

    void quit(int returnCode = 0);

protected:
    virtual void onStartup();
    virtual void onShutdown();
    virtual void onFrame(DurationMicros micros) {}

protected:
    bool m_gameplayPaused = false;
    UpdateFrame m_updateFrame;

private:
    void init();
    void handleWindowEvent(const SDL_Event & event);

private:
    std::string m_name;
    std::string m_prefixPath;
    bool        m_running = false;
    bool        m_initialized = false;
    int         m_returnCode = 0;

    FpsCounter m_fpsCounter;

    Optional<DrawContext> m_drawContext;

    SDL_Window *     m_displayWindow;
    SDL_Renderer *   m_displayRenderer;
    SDL_RendererInfo m_displayRendererInfo;
    SDL_GLContext    m_glContext;

    Optional<Input> m_input;

    int m_displayWidth = 1600;
    int m_displayHeight = 900;
};
}
