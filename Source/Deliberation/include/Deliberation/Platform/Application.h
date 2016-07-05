#pragma once

#include <string>

#include <SDL2/SDL.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API Application
{
public:
    Application(const std::string & name);
    virtual ~Application();

    Context & context();
    const Context & context() const;

    int run();

    void quit(int returnCode = 0);

protected:
    virtual void onStartup();
    virtual void onShutdown();
    virtual void onFrame(float seconds);

private:
    std::string         m_name;
    bool                m_running;
    int                 m_returnCode;

    Optional<Context>   m_context;

    SDL_Window *        m_displayWindow;
    SDL_Renderer *      m_displayRenderer;
    SDL_RendererInfo    m_displayRendererInfo;
    SDL_GLContext       m_glContext;
};

}

