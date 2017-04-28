#pragma once



#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{

class AmbientOcclusionBlur final
{
public:
    AmbientOcclusionBlur();
    AmbientOcclusionBlur(const Surface & occlusion,
                         const Surface & normalDepth);

    void schedule();

    Surface & output();
    const Surface & output() const;

private:
    bool                    m_initialized;
    Surface                 m_occlusion;
    Surface                 m_normalDepth;
    Texture                 m_output;
    ScreenSpaceEffect    m_effect;
};

}

