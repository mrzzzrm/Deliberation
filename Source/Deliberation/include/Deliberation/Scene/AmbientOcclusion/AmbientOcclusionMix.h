#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Scene/PostprocessingEffect.h>

namespace deliberation
{

class DELIBERATION_API AmbientOcclusionMix final
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
    PostprocessingEffect    m_effect;
};

}

