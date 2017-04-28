#pragma once

#include <string>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>



namespace deliberation
{

class StencilState final
{
public:
    struct Face
    {
        Face();
        Face(gl::GLenum func,
             gl::GLint ref,
             gl::GLuint readMask,
             gl::GLuint writeMask,
             gl::GLenum sfail,
             gl::GLenum dpfail,
             gl::GLenum dppass);

        gl::GLenum func = gl::GL_ALWAYS;
        gl::GLint ref = 0;
        gl::GLuint readMask = ~gl::GLuint(0);
        gl::GLuint writeMask = ~gl::GLuint(0);
        gl::GLenum sfail = gl::GL_KEEP;
        gl::GLenum dpfail = gl::GL_KEEP;
        gl::GLenum dppass = gl::GL_KEEP;
    };

public:
    StencilState();
    StencilState(const Face & frontAndBack);
    StencilState(gl::GLenum func,
                 gl::GLint ref,
                 gl::GLuint readMask,
                 gl::GLuint writeMask,
                 gl::GLenum sfail,
                 gl::GLenum dpfail,
                 gl::GLenum dppass);
    StencilState(const Face & front, const Face & back);

    bool differentFaceFuncs() const;
    bool differentFaceMasks() const;
    bool differentFaceOps() const;

    const Face & frontFace() const;
    const Face & backFace() const;

    bool enabled() const;
    gl::GLenum func(gl::GLenum face = gl::GL_FRONT_AND_BACK) const;
    gl::GLint ref(gl::GLenum face = gl::GL_FRONT_AND_BACK) const;
    gl::GLuint readMask(gl::GLenum face = gl::GL_FRONT_AND_BACK) const;
    gl::GLuint writeMask(gl::GLenum face = gl::GL_FRONT_AND_BACK) const;
    gl::GLenum sfail(gl::GLenum face = gl::GL_FRONT_AND_BACK) const;
    gl::GLenum dpfail(gl::GLenum face = gl::GL_FRONT_AND_BACK) const;
    gl::GLenum dppass(gl::GLenum face = gl::GL_FRONT_AND_BACK) const;

    void setEnabled(bool enabled);
    void setFunc(gl::GLenum func, gl::GLenum face = gl::GL_FRONT_AND_BACK);
    void setRef(gl::GLint ref, gl::GLenum face = gl::GL_FRONT_AND_BACK);
    void setReadMask(gl::GLuint readMask, gl::GLenum face = gl::GL_FRONT_AND_BACK);
    void setWriteMask(gl::GLuint writeMask, gl::GLenum face = gl::GL_FRONT_AND_BACK);
    void setSFail(gl::GLenum sfail, gl::GLenum face = gl::GL_FRONT_AND_BACK);
    void setDPFail(gl::GLenum dpfail, gl::GLenum face = gl::GL_FRONT_AND_BACK);
    void setDPPass(gl::GLenum dppass, gl::GLenum face = gl::GL_FRONT_AND_BACK);

    void apply() const;
    void undo() const;

    std::string toString() const;

private:
    bool m_enabled;
    uint32_t m_differentFrontAndBackBits;
    Face m_faces[2];
};

}

