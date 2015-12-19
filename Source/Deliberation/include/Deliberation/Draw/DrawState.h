#pragma once

#include <string>

#include <Deliberation/Core/Viewport.h>
#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/BlendState.h>
#include <Deliberation/Draw/CullState.h>
#include <Deliberation/Draw/DepthState.h>
#include <Deliberation/Draw/RasterizerState.h>
#include <Deliberation/Draw/StencilState.h>

namespace deliberation
{

class DrawState final
{
public:
    DrawState();

    gl::GLenum primitive() const;

    gl::GLenum provokingVertex() const;

    BlendState & blendState();
    const BlendState & blendState() const;

    CullState & cullState();
    const CullState & cullState() const;

    RasterizerState & rasterizerState();
    const RasterizerState & rasterizerState() const;

    DepthState & depthState();
    const DepthState & depthState() const;

    StencilState & stencilState();
    const StencilState & stencilState() const;

    bool hasViewport() const;
    const Viewport & viewport() const;

    void setPrimitive(gl::GLenum primitive);

    void setProvokingVertex(gl::GLenum provokingVertex);

    void setBlendState(const BlendState & blendState);
    void setCullState(const CullState & cullState);
    void setRasterizerState(const RasterizerState & rasterizerState);
    void setDepthState(const DepthState & depthState);
    void setStencilState(const StencilState & stencilState);

    void setViewport(const Viewport & viewport);

    std::string toString() const;

private:
    gl::GLenum m_primitive; // TODO Should be part of rasterizer state?!

    gl::GLenum m_provokingVertex;

    BlendState m_blendState;
    CullState m_cullState;
    DepthState m_depthState;
    RasterizerState m_rasterizerState;
    StencilState m_stencilState;

    Optional<Viewport> m_viewport;
};

}

