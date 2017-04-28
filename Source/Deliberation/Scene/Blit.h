#pragma once



#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/Viewport.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Draw.h>

namespace deliberation
{

/*
    TODO
        Blit from Surface to another Surface
*/

class Blit final
{
public:
    Blit();
    Blit(const Surface & source);
    Blit(const Surface & source, const Viewport & viewport);

    void schedule();

private:
    Optional<Surface>   m_source;
    Optional<Draw>      m_draw;
    Viewport            m_viewport;
};

}

