#pragma once

#include <memory>

#include <SDL.h>

#include <Deliberation/Core/InputBase.h>



namespace deliberation
{

class DrawContext;

class Input:
    public InputBase
{
public:
    Input(const DrawContext & drawContext);

    void onSDLInputEvent(const SDL_Event & event);

private:
    const DrawContext &  m_drawContext;
};

}