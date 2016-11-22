#pragma once

#include <string>

#include <SDL2/SDL.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Platform/InputAdapter.h>

namespace deliberation
{

class DELIBERATION_API Application
{
public:
    Application(const std::string & name,
                const std::string & prefixPath = "Deliberation2");
    virtual ~Application();

    InputAdapterBase & inputAdapter();
    const InputAdapterBase & inputAdapter() const;

    Context & context();
    const Context & context() const;

    int run(int argc,
            char ** argv);

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

    Optional<Context>   m_context;

    SDL_Window *        m_displayWindow;
    SDL_Renderer *      m_displayRenderer;
    SDL_RendererInfo    m_displayRendererInfo;
    SDL_GLContext       m_glContext;

    InputAdapter        m_inputAdapter;
};

}

