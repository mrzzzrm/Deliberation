#pragma once

#include <SDL.h>

#include <Deliberation/Core/InputBase.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Context;

class DELIBERATION_API Input:
    public InputBase
{
public:
    Input(const Context & context);

    void onSDLInputEvent(const SDL_Event & event);

private:
    const Context & m_context;
};

}