#pragma once

namespace deliberation
{
enum class RenderPhase
{
    Forward,
    GBuffer,
    PostGBuffer, // e.g. SSAO
    PreLighting, // e.g. ShadowMaps
    Lighting,
    Alpha,
    PostAlpha,
    Hdr,

    Overlay,

    Count
};
}