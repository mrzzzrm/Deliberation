#pragma once

#include <array>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

namespace deliberation
{

class GLStateManager final
{
public:
    GLStateManager();

    void enableTextureCubeMapSeamless(bool enabled);
    void enableDepthTest(bool enabled);
    void setDepthMask(bool enabled);
    void enableBlend(bool enabled);
    void setBlendEquation(gl::GLenum equation);
    void setBlendFunc(gl::GLenum sfactor, gl::GLenum dfactor);
    void enableCullFace(bool enabled);
    void setCullFace(gl::GLenum mode);
    void setPointSize(float size);
    void setLineWidth(float width);
    void enableStencilTest(bool enabled);
    void setStencilFunc(gl::GLenum func, int ref, unsigned int mask);
    void setStencilMask(unsigned int mask);
    void setStencilOp(gl::GLenum sfail, gl::GLenum dpfail, gl::GLenum dppass);
    void setStencilFuncSeparate(gl::GLenum face, gl::GLenum func, int ref, unsigned int mask);
    void setStencilMaskSeparate(gl::GLenum face, unsigned int mask);
    void setStencilOpSeparate(gl::GLenum face, gl::GLenum sfail, gl::GLenum dpfail, gl::GLenum dppass);

    void bindBuffer(gl::GLenum target, gl::GLuint buffer);
    void deleteBuffer(gl::GLuint buffer);

private:
    void applyEnableDisableState(gl::GLenum state, bool & current, bool target);

private:
    enum BufferTarget
    {
        ArrayBufferTarget = 0,
        CopyReadBufferTarget,
        CopyWriteBufferTarget,
        ElementArrayBufferTarget,
        PixelPackBufferTarget,
        PixelUnpackBufferTarget,
        TextureBufferTarget,
        TransformFeedbackBufferTarget,
        UniformBufferTarget,

        BufferTargetCount
    };

private:
    bool m_glTextureCubeMapSeamless;
    bool m_glDepthTest;
    bool m_glDepthMask;
    bool m_glBlend;
    gl::GLenum m_glBlendEquation;
    gl::GLenum m_glBlendFuncSFactor;
    gl::GLenum m_glBlendFuncDFactor;
    bool m_glCullFace;
    gl::GLenum m_glCullFaceMode;
    float m_glPointSize;
    float m_glLineWidth;
    bool m_glStencilTest;
    gl::GLenum m_glStencilFunc[2];
    int m_glStencilRef[2];
    unsigned int m_glStencilReadMask[2];
    unsigned int m_glStencilWriteMask[2];
    gl::GLenum m_glStencilSFail[2];
    gl::GLenum m_glStencilDPFail[2];
    gl::GLenum m_glStencilDPPass[2];
    std::array<gl::GLuint, BufferTargetCount> m_boundBuffers;
};

}


