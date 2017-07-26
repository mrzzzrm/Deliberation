#include "FramebufferImpl.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/GL/GLStateManager.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/Draw/Surface.h>

#include <Deliberation/Draw/Detail/TextureImpl.h>

namespace
{
using namespace deliberation;

void RenderTargetFromDesc(
    DrawContext &            drawContext,
    RenderTarget &           renderTarget,
    const FramebufferDesc &  framebufferDesc,
    const RenderTargetDesc & renderTargetDesc)
{
    renderTarget.name = renderTargetDesc.name;

    if (renderTargetDesc.format != PixelFormat_None)
    {
        auto texture = drawContext.createTexture2D(
            framebufferDesc.width,
            framebufferDesc.height,
            renderTargetDesc.format);
        renderTarget.surface = texture.surface(0);
    }
    else
    {
        renderTarget.surface = renderTargetDesc.surface;
    }
}
}

namespace deliberation
{
std::shared_ptr<FramebufferImpl>
FramebufferImpl::backbuffer(DrawContext & drawContext, u32 width, u32 height)
{
    auto result = std::make_shared<FramebufferImpl>(drawContext);
    result->isBackbuffer = true;
    result->width = width;
    result->height = height;
    result->name = "Backbuffer";

    return result;
}

std::shared_ptr<FramebufferImpl> FramebufferImpl::custom(
    DrawContext & drawContext, const FramebufferDesc & framebufferDesc)
{
    auto result = std::make_shared<FramebufferImpl>(drawContext);
    result->isBackbuffer = false;
    result->width = framebufferDesc.width;
    result->height = framebufferDesc.height;

    // Init RenderTargets, create Textures where necessary
    result->colorTargets.reserve(framebufferDesc.colorTargetDescs.size());
    for (const auto & renderTargetDesc : framebufferDesc.colorTargetDescs)
    {
        RenderTarget colorTarget;

        RenderTargetFromDesc(
            drawContext, colorTarget, framebufferDesc, renderTargetDesc);

        result->colorTargets.emplace_back(colorTarget);
    }

    if (framebufferDesc.depthTargetDesc)
    {
        RenderTarget depthTarget;

        RenderTargetFromDesc(
            drawContext,
            depthTarget,
            framebufferDesc,
            *framebufferDesc.depthTargetDesc);

        result->depthTarget = depthTarget;
    }

    // Setup GL framebuffer
    auto & glStateManager = drawContext.m_glStateManager;

    glStateManager.genFramebuffers(1, &result->glName);
    Assert(result->glName != 0, "Failed to create GL Framebuffer Object");

    glStateManager.bindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, result->glName);

    for (auto a = 0u; a < result->colorTargets.size(); a++)
    {
        const auto & target = result->colorTargets[a];

        glStateManager.framebufferTexture2D(
            gl::GL_DRAW_FRAMEBUFFER,
            (gl::GLenum)((u32)gl::GL_COLOR_ATTACHMENT0 + a),
            target.surface.m_impl->glTarget,
            target.surface.m_impl->textureImpl->glName,
            0);
    }

    if (result->depthTarget)
    {
        glStateManager.framebufferTexture2D(
            gl::GL_DRAW_FRAMEBUFFER,
            gl::GL_DEPTH_ATTACHMENT,
            gl::GL_TEXTURE_2D,
            result->depthTarget->surface.m_impl->textureImpl->glName,
            0);
    }

    // Framebuffer should be complete now
    auto status = gl::glCheckFramebufferStatus(gl::GL_DRAW_FRAMEBUFFER);
    Assert(status != gl::GL_FRAMEBUFFER_UNDEFINED, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER, "");
    Assert(status != gl::GL_FRAMEBUFFER_UNSUPPORTED, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE, "");
    Assert(status != gl::GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS, "");
    Assert(status == gl::GL_FRAMEBUFFER_COMPLETE, "");

    result->name = framebufferDesc.name;

    return result;
}

FramebufferImpl::FramebufferImpl(DrawContext & drawContext)
    : drawContext(drawContext)
{
}

FramebufferImpl::~FramebufferImpl()
{
    auto & glStateManager = drawContext.m_glStateManager;
    glStateManager.deleteFramebuffers(1, &glName);
}

void FramebufferImpl::bind(const std::vector<gl::GLenum> & drawBuffers)
{
    auto & glStateManager = drawContext.m_glStateManager;

    if (isBackbuffer)
    {
        glStateManager.bindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, 0);

        if (drawBuffers.empty()) glStateManager.setDrawBuffer(gl::GL_BACK);
        else glStateManager.setDrawBuffers(drawBuffers);
    }
    else
    {
        glStateManager.bindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, glName);
        glStateManager.setDrawBuffers(drawBuffers);
    }
}

size_t
FramebufferImpl::colorTargetIndex(const std::string & name, bool * found) const
{
    auto iter = std::find_if(
        colorTargets.begin(),
        colorTargets.end(),
        [&name](const RenderTarget & colorTarget) {
            return colorTarget.name == name;
        });

    if (found)
    {
        *found = iter != colorTargets.end();
    }
    else
    {
        Assert(
            iter != colorTargets.end(),
            "No such color target '" + name + "' in Framebuffer");
    }

    return (size_t)(iter - colorTargets.begin());
}

std::string FramebufferImpl::toString() const
{
    std::stringstream s;
    s << "Framebuffer '" << name << "'{\n";
    if (!colorTargets.empty())
    {
        s << "  ColorTargets {\n";
        for (const auto & rt : colorTargets)
        {
            s << "    " << rt.name << ": " << rt.surface.toString() << "\n";
        }
        s << "  }\n";
    }
    if (depthTarget)
    {
        s << "  DepthTarget: " << depthTarget->surface.toString() << "\n";
    }
    s << "}\n";

    return s.str();
}
}
