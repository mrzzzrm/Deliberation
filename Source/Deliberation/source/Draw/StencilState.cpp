#include "StencilState.h"

#include <glbinding/gl/functions.h>

#include "Core/Assert.h"

namespace
{

constexpr int FUNC_SHIFT            = 0;
constexpr int REF_SHIFT             = 1;
constexpr int READMASK_SHIFT        = 2;
constexpr int WRITEMASK_SHIFT       = 3;
constexpr int SFAIL_SHIFT           = 4;
constexpr int DPFAIL_SHIFT          = 5;
constexpr int DPPASS_SHIFT          = 6;

constexpr uint32_t FUNC_BIT         = 1 << FUNC_SHIFT;
constexpr uint32_t REF_BIT          = 1 << REF_SHIFT;
constexpr uint32_t READMASK_BIT     = 1 << READMASK_SHIFT;
constexpr uint32_t WRITEMASK_BIT    = 1 << WRITEMASK_SHIFT;
constexpr uint32_t SFAIL_BIT        = 1 << SFAIL_SHIFT;
constexpr uint32_t DPFAIL_BIT       = 1 << DPFAIL_SHIFT;
constexpr uint32_t DPPASS_BIT       = 1 << DPPASS_SHIFT;

constexpr uint32_t FUNC_MASK        = FUNC_BIT | REF_BIT | READMASK_BIT;
constexpr uint32_t MASK_MASK        = WRITEMASK_BIT;
constexpr uint32_t OP_MASK          = SFAIL_BIT | DPFAIL_BIT | DPPASS_BIT;

unsigned int faceToIndex(gl::GLenum face)
{
    Assert(face == gl::GL_FRONT ||
           face == gl::GL_BACK ||
           face == gl::GL_FRONT_AND_BACK,
           "");
    return face == gl::GL_BACK ? 1 : 0;
}

}

StencilState::Face::Face() = default;

StencilState::Face::Face(gl::GLenum func,
                         gl::GLint ref,
                         gl::GLuint readMask,
                         gl::GLuint writeMask,
                         gl::GLenum sfail,
                         gl::GLenum dpfail,
                         gl::GLenum dppass):
                        func(func),
                        ref(ref),
                        readMask(readMask),
                        writeMask(writeMask),
                        sfail(sfail),
                        dpfail(dpfail),
                        dppass(dppass)
{
}

StencilState::StencilState():
    m_enabled(false),
    m_differentFrontAndBackBits(0)
{

}

StencilState::StencilState(const Face & frontAndBack):
    m_enabled(true),
    m_differentFrontAndBackBits(0)
{
    m_faces[0] = frontAndBack;
    m_faces[1] = frontAndBack;
}

StencilState::StencilState(gl::GLenum func,
                           gl::GLint ref,
                           gl::GLuint readMask,
                           gl::GLuint writeMask,
                           gl::GLenum sfail,
                           gl::GLenum dpfail,
                           gl::GLenum dppass):
    StencilState(Face(func, ref, readMask, writeMask, sfail, dpfail, dppass))
{

}

StencilState::StencilState(const Face & front, const Face & back):
    m_enabled(true),
    m_differentFrontAndBackBits(~0)
{
    m_faces[0] = front;
    m_faces[1] = back;
}

bool StencilState::differentFaceFuncs() const
{
    return m_differentFrontAndBackBits & FUNC_MASK;
}

bool StencilState::differentFaceMasks() const
{
    return m_differentFrontAndBackBits & MASK_MASK;
}

bool StencilState::differentFaceOps() const
{
    return m_differentFrontAndBackBits & OP_MASK;
}

const StencilState::Face & StencilState::frontFace() const
{
    return m_faces[0];
}

const StencilState::Face & StencilState::backFace() const
{
    return m_faces[1];
}

bool StencilState::enabled() const
{
    return m_enabled;
}

gl::GLenum StencilState::func(gl::GLenum face) const
{
    Assert(face != gl::GL_FRONT_AND_BACK || !(m_differentFrontAndBackBits & FUNC_BIT), "");
    return m_faces[faceToIndex(face)].func;
}

gl::GLint StencilState::ref(gl::GLenum face) const
{
    Assert(face != gl::GL_FRONT_AND_BACK || !(m_differentFrontAndBackBits & REF_BIT), "");
    return m_faces[faceToIndex(face)].ref;
}

gl::GLuint StencilState::readMask(gl::GLenum face) const
{
    Assert(face != gl::GL_FRONT_AND_BACK || !(m_differentFrontAndBackBits & READMASK_BIT), "");
    return m_faces[faceToIndex(face)].readMask;
}

gl::GLuint StencilState::writeMask(gl::GLenum face) const
{
    Assert(face != gl::GL_FRONT_AND_BACK || !(m_differentFrontAndBackBits & WRITEMASK_BIT), "");
    return m_faces[faceToIndex(face)].writeMask;
}

gl::GLenum StencilState::sfail(gl::GLenum face) const
{
    Assert(face != gl::GL_FRONT_AND_BACK || !(m_differentFrontAndBackBits & SFAIL_BIT), "");
    return m_faces[faceToIndex(face)].sfail;
}

gl::GLenum StencilState::dpfail(gl::GLenum face) const
{
    Assert(face != gl::GL_FRONT_AND_BACK || !(m_differentFrontAndBackBits & DPFAIL_BIT), "");
    return m_faces[faceToIndex(face)].dpfail;
}

gl::GLenum StencilState::dppass(gl::GLenum face) const
{
    Assert(face != gl::GL_FRONT_AND_BACK || !(m_differentFrontAndBackBits & DPPASS_BIT), "");
    return m_faces[faceToIndex(face)].dppass;
}

void StencilState::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void StencilState::setFunc(gl::GLenum func, gl::GLenum face)
{
    if (face == gl::GL_FRONT_AND_BACK)
    {
        m_faces[0].func = func;
        m_faces[1].func = func;
    }
    else
    {
        m_faces[faceToIndex(face)].func = func;
    }
    m_differentFrontAndBackBits = (m_faces[0].func != m_faces[1].func ? 1 : 0) << FUNC_SHIFT;
}

void StencilState::setRef(gl::GLint ref, gl::GLenum face)
{
    if (face == gl::GL_FRONT_AND_BACK)
    {
        m_faces[0].ref = ref;
        m_faces[1].ref = ref;
    }
    else
    {
        m_faces[faceToIndex(face)].ref = ref;
    }
    m_differentFrontAndBackBits = (m_faces[0].ref != m_faces[1].ref ? 1 : 0) << REF_SHIFT;
}

void StencilState::setReadMask(gl::GLuint readMask, gl::GLenum face)
{
    if (face == gl::GL_FRONT_AND_BACK)
    {
        m_faces[0].readMask = readMask;
        m_faces[1].readMask = readMask;
    }
    else
    {
        m_faces[faceToIndex(face)].readMask = readMask;
    }
    m_differentFrontAndBackBits = (m_faces[0].readMask != m_faces[1].readMask ? 1 : 0) << READMASK_SHIFT;
}

void StencilState::setWriteMask(gl::GLuint writeMask, gl::GLenum face)
{
    if (face == gl::GL_FRONT_AND_BACK)
    {
        m_faces[0].writeMask = writeMask;
        m_faces[1].writeMask = writeMask;
    }
    else
    {
        m_faces[faceToIndex(face)].writeMask = writeMask;
    }
    m_differentFrontAndBackBits = (m_faces[0].writeMask != m_faces[1].writeMask ? 1 : 0) << WRITEMASK_SHIFT;
}

void StencilState::setSFail(gl::GLenum sfail, gl::GLenum face)
{
    if (face == gl::GL_FRONT_AND_BACK)
    {
        m_faces[0].sfail = sfail;
        m_faces[1].sfail = sfail;
    }
    else
    {
        m_faces[faceToIndex(face)].sfail = sfail;
    }
    m_differentFrontAndBackBits = (m_faces[0].sfail != m_faces[1].sfail ? 1 : 0) << SFAIL_SHIFT;
}

void StencilState::setDPFail(gl::GLenum dpfail, gl::GLenum face)
{
    if (face == gl::GL_FRONT_AND_BACK)
    {
        m_faces[0].dpfail = dpfail;
        m_faces[1].dpfail = dpfail;
    }
    else
    {
        m_faces[faceToIndex(face)].dpfail = dpfail;
    }
    m_differentFrontAndBackBits = (m_faces[0].dpfail != m_faces[1].dpfail ? 1 : 0) << DPFAIL_SHIFT;
}

void StencilState::setDPPass(gl::GLenum dppass, gl::GLenum face)
{
    if (face == gl::GL_FRONT_AND_BACK)
    {
        m_faces[0].dppass = dppass;
        m_faces[1].dppass = dppass;
    }
    else
    {
        m_faces[faceToIndex(face)].dppass = dppass;
    }
    m_differentFrontAndBackBits = (m_faces[0].dppass != m_faces[1].dppass ? 1 : 0) << DPPASS_SHIFT;
}

std::string StencilState::toString() const
{
    return "Unimplemented";
}

