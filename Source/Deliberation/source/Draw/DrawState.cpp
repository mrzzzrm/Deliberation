#include <Deliberation/Draw/DrawState.h>

#include <sstream>

#include <Deliberation/Core/Assert.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

namespace deliberation
{

DrawState::DrawState():
    m_primitive(gl::GL_TRIANGLES),
    m_provokingVertex(gl::GL_FIRST_VERTEX_CONVENTION)
{
}

gl::GLenum DrawState::primitive() const
{
    return m_primitive;
}

gl::GLenum DrawState::provokingVertex() const
{
    return m_provokingVertex;
}

BlendState & DrawState::blendState()
{
    return m_blendState;
}

const BlendState & DrawState::blendState() const
{
    return m_blendState;
}

CullState & DrawState::cullState()
{
    return m_cullState;
}

const CullState & DrawState::cullState() const
{
    return m_cullState;
}

RasterizerState & DrawState::rasterizerState()
{
    return m_rasterizerState;
}

const RasterizerState & DrawState::rasterizerState() const
{
    return m_rasterizerState;
}

DepthState & DrawState::depthState()
{
    return m_depthState;
}

const DepthState & DrawState::depthState() const
{
    return m_depthState;
}

StencilState & DrawState::stencilState()
{
    return m_stencilState;
}

const StencilState & DrawState::stencilState() const
{
    return m_stencilState;
}

bool DrawState::hasViewport() const
{
    return m_viewport.engaged();
}

const Viewport & DrawState::viewport() const
{
    Assert(hasViewport(), "");
    return m_viewport.get();
}

void DrawState::setPrimitive(gl::GLenum primitive)
{
    m_primitive = primitive;
}

void DrawState::setProvokingVertex(gl::GLenum provokingVertex)
{
    m_provokingVertex = provokingVertex;
}

void DrawState::setBlendState(const BlendState & blendState)
{
    m_blendState = blendState;
}

void DrawState::setCullState(const CullState & cullState)
{
    m_cullState = cullState;
}

void DrawState::setRasterizerState(const RasterizerState & rasterizerState)
{
    m_rasterizerState = rasterizerState;
}

void DrawState::setDepthState(const DepthState & depthState)
{
    m_depthState = depthState;
}

void DrawState::setStencilState(const StencilState & stencilState)
{
    m_stencilState = stencilState;
}

void DrawState::setViewport(const Viewport & viewport)
{
    m_viewport.reset(viewport);
}

std::string DrawState::toString() const
{
    std::stringstream stream;

    stream <<
        "Primitive " << m_primitive << std::endl <<
        "BlendState: " << m_blendState.toString() << std::endl <<
        "CullState: " << m_cullState.toString() << std::endl <<
        "RasterizerState: " << m_rasterizerState.toString() << std::endl <<
        "DepthState: " << m_depthState.toString() << std::endl <<
        "StencilState: " << m_stencilState.toString() << std::endl;

    return stream.str();
}

}

