#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Scene/PostprocessingEffect.h>

namespace deliberation
{

class DELIBERATION_API AmbientOcclusionBlur final
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
    PostprocessingEffect    m_effect;
};

}

