#pragma once

#include <SDL2/SDL.h>

#include <Deliberation/Core/InputAdapterBase.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API InputAdapter:
    public InputAdapterBase
{
public:
    void onSDLInputEvent(const SDL_Event & event);
};

}