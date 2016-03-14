#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/Surface.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/Uniform.h>

#include <Deliberation/Scene/AmbientOcclusion/AmbientOcclusionBlur.h>
#include <Deliberation/Scene/AmbientOcclusion/AmbientOcclusionMix.h>
#include <Deliberation/Scene/PostprocessingEffect.h>

namespace deliberation
{

class Camera3D;

class DELIBERATION_API AmbientOcclusion final
{
public:
    AmbientOcclusion();
    AmbientOcclusion(const Surface & normalDepth,
                     const Surface & color,
                     const Surface & positionVS,
                     const Camera3D & camera);

    Surface & rawOcclusion();
    const Surface & rawOcclusion() const;

    Surface & blurredOcclusion();
    const Surface & blurredOcclusion() const;

    Surface & output();
    const Surface & output() const;

    void setSampleRadius(float radius);

    void schedule();

private:
    bool                    m_initialized;

    Surface                 m_normalDepth;
    Surface                 m_color;
    Surface                 m_positionVS;
    const Camera3D *        m_camera;
    Texture                 m_output;
    PostprocessingEffect    m_effect;
    Texture                 m_rotation;

    Uniform                 m_viewProjection;
    Uniform                 m_projection;
    Uniform                 m_rotationCoordScale;
    Uniform                 m_sampleRadius;

    AmbientOcclusionBlur    m_blur;
    AmbientOcclusionMix     m_mix;
};

}

