#pragma once

#include <array>
#include <memory>
#include <unordered_map>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class GLFramebuffer;
class GLFramebufferDesc;

class DELIBERATION_API GLStateManager final
{
public:
    GLStateManager();

    /*
        Raw OpenGL calls
    */
    void enableTextureCubeMapSeamless(bool enabled);
    void enableDepthTest(bool enabled);
    void setDepthFunc(gl::GLenum func);
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
    void setClearColor(gl::GLclampf red, gl::GLclampf green, gl::GLclampf blue, gl::GLclampf alpha);
    void setClearDepth(gl::GLclampd depth);
    void setClearStencil(gl::GLint s);
    void genFramebuffers(gl::GLsizei n, gl::GLuint * ids);
    void deleteFramebuffers(gl::GLsizei n, gl::GLuint * framebuffers);
    void bindFramebuffer(gl::GLenum target, gl::GLuint framebuffer);
    void framebufferTexture2D(gl::GLenum target, gl::GLenum attachment, gl::GLenum textarget, gl::GLuint texture, gl::GLint level);
    void bindBuffer(gl::GLenum target, gl::GLuint buffer);
    void deleteBuffer(gl::GLuint buffer);
    void setViewport(gl::GLint x, gl::GLint y, gl::GLsizei width, gl::GLsizei height);
    void genQueries(gl::GLsizei n, gl::GLuint * ids);
    void deleteQueries(gl::GLsizei n, gl::GLuint * ids);
    void beginQuery(gl::GLenum target, gl::GLuint id);
    void endQuery(gl::GLenum target);
    void useProgram(gl::GLuint program);
    void setActiveTexture(gl::GLuint textureUnit);
    void bindTexture(gl::GLenum target, gl::GLuint texture);

    /*
        Framebuffer caching
    */
    std::shared_ptr<GLFramebuffer> framebuffer(const GLFramebufferDesc & desc);

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

    enum FramebufferTarget
    {
        ReadFramebufferTarget = 0,
        DrawFramebufferTarget,

        FramebufferTargetCount
    };

    enum QueryTarget
    {
        QueryTimeElapsedTarget = 0,
        QuerySamplesPassedTarget,
        QueryAnySamplesPassedTarget,
        QueryPrimitivesGeneratedTarget,
        QueryTransformFeedbackPrimitivesTarget,

        QueryTargetCount
    };

    enum TextureTarget
    {
        Texture1d = 0,
        Texture2d,
        Texture3d,
        Texture1dArray,
        Texture2dArray,
        TextureRectangle,
        TextureCubeMap,
        TextureBuffer,
        Texture2dMultisample,
        Texture2dMultisampleArray,

        TextureTargetCount
    };

private:
    QueryTarget glEnumToQueryTarget(gl::GLenum e) const;

private:
    /*
        Raw OpenGL State
    */
    bool                                m_glTextureCubeMapSeamless;
    bool                                m_glDepthTest;
    gl::GLenum                          m_glDepthFunc;
    bool                                m_glDepthMask;
    bool                                m_glBlend;
    gl::GLenum                          m_glBlendEquation;
    gl::GLenum                          m_glBlendFuncSFactor;
    gl::GLenum                          m_glBlendFuncDFactor;
    bool                                m_glCullFace;
    gl::GLenum                          m_glCullFaceMode;
    float                               m_glPointSize;
    float                               m_glLineWidth;
    bool                                m_glStencilTest;
    gl::GLenum                          m_glStencilFunc[2];
    int                                 m_glStencilRef[2];
    unsigned int                        m_glStencilReadMask[2];
    unsigned int                        m_glStencilWriteMask[2];
    gl::GLenum                          m_glStencilSFail[2];
    gl::GLenum                          m_glStencilDPFail[2];
    gl::GLenum                          m_glStencilDPPass[2];
    std::array<gl::GLuint,
               BufferTargetCount>       m_boundBuffers;
    gl::GLclampf                        m_glClearColorRed;
    gl::GLclampf                        m_glClearColorGreen;
    gl::GLclampf                        m_glClearColorBlue;
    gl::GLclampf                        m_glClearColorAlpha;
    gl::GLclampd                        m_glClearDepth;
    gl::GLint                           m_glClearStencil;
    std::array<gl::GLuint,
               FramebufferTargetCount>  m_boundFramebuffers;
    gl::GLint                           m_glViewportX;
 	gl::GLint                           m_glViewportY;
 	gl::GLsizei                         m_glViewportWidth;
 	gl::GLsizei                         m_glViewportHeight;
 	std::array<gl::GLuint,
               QueryTargetCount>        m_activeQueries;
    gl::GLuint                          m_program;
    std::array<gl::GLuint , (size_t)TextureTargetCount>
                                        m_boundTextures;
    gl::GLuint                          m_activeTextureUnit;

    /*

    */
    std::unordered_map<std::size_t,
                       std::weak_ptr<GLFramebuffer>> m_glFramebuffers;
};

}


