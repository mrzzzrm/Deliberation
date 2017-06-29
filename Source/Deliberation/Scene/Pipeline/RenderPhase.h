#pragma once

namespace deliberation
{

/**
 * Sync order with function below
 */
enum class RenderPhase
{
    GBuffer,
    PostGBuffer, // e.g. SSAO
    PreLighting, // e.g. ShadowMaps
    Lighting,
    Forward,
    Alpha,
    PostAlpha,
    PreHdr,
    Hdr,

    PreOverlay,
    Overlay,

    Count
};

inline std::string RenderPhaseToString(RenderPhase phase)
{
    const auto index = (int)phase;

    static const char * names[] = {
        "GBuffer",
        "PostGBuffer",
        "PreLighting",
        "Lighting",
        "Forward",
        "Alpha",
        "PostAlpha",
        "PreHdr",
        "Hdr",
        "PreOverlay",
        "Overlay"
    };

    return names[index];
}
}