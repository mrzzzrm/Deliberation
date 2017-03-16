#pragma once

#include <string>

#include <SDL2/SDL.h>

#include <Deliberation/Core/FpsCounter.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Platform/Input.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class DELIBERATION_API Application
{
public:
    Application(const std::string & name,
                const std::string & prefixPath = ".");
    virtual ~Application();

    InputBase & input();
    const InputBase & input() const;

    Context & context();
    const Context & context() const;

    float fps() const;

    int run(int argc, char ** argv);

    void quit(int returnCode = 0);

protected:
    virtual void onStartup();
    virtual void onShutdown();
    virtual void onFrame(float seconds);

private:
    void init();

private:
    std::string         m_name;
    std::string         m_prefixPath;
    bool                m_running = false;
    bool                m_initialized = false;
    int                 m_returnCode = 0;

    FpsCounter          m_fpsCounter;

    Optional<Context>   m_context;

    SDL_Window *        m_displayWindow;
    SDL_Renderer *      m_displayRenderer;
    SDL_RendererInfo    m_displayRendererInfo;
    SDL_GLContext       m_glContext;

    Optional<Input>     m_input;
};

}

