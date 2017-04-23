#pragma once

namespace deliberation
{

enum class RenderPhase
{
    GBuffer,
    PostGBuffer, // e.g. SSAO
    PreLighting, // e.g. ShadowMaps
    Lighting,
    HDR,

    Count
};

}