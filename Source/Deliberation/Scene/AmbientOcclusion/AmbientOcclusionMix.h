#pragma once



#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{

class AmbientOcclusionMix final
{
public:
    AmbientOcclusionMix();
    AmbientOcclusionMix(const Surface & occlusion,
                        const Surface & color);

    void schedule();

    Surface & output();
    const Surface & output() const;

private:
    bool                    m_initialized;
    Texture                 m_output;
    ScreenSpaceEffect    m_effect;
};

}

