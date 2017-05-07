#include <Deliberation/Draw/StencilState.h>

#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

namespace
{
using namespace deliberation;

constexpr int FUNC_SHIFT = 0;
constexpr int REF_SHIFT = 1;
constexpr int READMASK_SHIFT = 2;
constexpr int WRITEMASK_SHIFT = 3;
constexpr int SFAIL_SHIFT = 4;
constexpr int DPFAIL_SHIFT = 5;
constexpr int DPPASS_SHIFT = 6;

constexpr u32 FUNC_BIT = 1 << FUNC_SHIFT;
constexpr u32 REF_BIT = 1 << REF_SHIFT;
constexpr u32 READMASK_BIT = 1 << READMASK_SHIFT;
constexpr u32 WRITEMASK_BIT = 1 << WRITEMASK_SHIFT;
constexpr u32 SFAIL_BIT = 1 << SFAIL_SHIFT;
constexpr u32 DPFAIL_BIT = 1 << DPFAIL_SHIFT;
constexpr u32 DPPASS_BIT = 1 << DPPASS_SHIFT;

constexpr u32 FUNC_MASK = FUNC_BIT | REF_BIT | READMASK_BIT;
constexpr u32 MASK_MASK = WRITEMASK_BIT;
constexpr u32 OP_MASK = SFAIL_BIT | DPFAIL_BIT | DPPASS_BIT;

unsigned int faceToIndex(StencilFace face)
{
    return face == StencilFace::Back ? 1 : 0;
}
}

namespace deliberation
{
StencilState::Face::Face() = default;

StencilState::Face::Face(
    StencilFunction func,
    u32             ref,
    u32             readMask,
    u32             writeMask,
    StencilOp       sfail,
    StencilOp       dpfail,
    StencilOp       dppass)
    : func(func)
    , ref(ref)
    , readMask(readMask)
    , writeMask(writeMask)
    , sfail(sfail)
    , dpfail(dpfail)
    , dppass(dppass)
{
}

StencilState::StencilState() : m_enabled(false), m_differentFrontAndBackBits(0)
{
}

StencilState::StencilState(const Face & frontAndBack)
    : m_enabled(true), m_differentFrontAndBackBits(0)
{
    m_faces[0] = frontAndBack;
    m_faces[1] = frontAndBack;
}

StencilState::StencilState(
    StencilFunction func,
    u32             ref,
    u32             readMask,
    u32             writeMask,
    StencilOp       sfail,
    StencilOp       dpfail,
    StencilOp       dppass)
    : StencilState(Face(func, ref, readMask, writeMask, sfail, dpfail, dppass))
{
}

StencilState::StencilState(const Face & front, const Face & back)
    : m_enabled(true), m_differentFrontAndBackBits(~0)
{
    m_faces[0] = front;
    m_faces[1] = back;
}

bool StencilState::differentFaceFuncs() const
{
    return (m_differentFrontAndBackBits & FUNC_MASK) != 0;
}

bool StencilState::differentFaceMasks() const
{
    return (m_differentFrontAndBackBits & MASK_MASK) != 0;
}

bool StencilState::differentFaceOps() const
{
    return (m_differentFrontAndBackBits & OP_MASK) != 0;
}

const StencilState::Face & StencilState::frontFace() const
{
    return m_faces[0];
}

const StencilState::Face & StencilState::backFace() const { return m_faces[1]; }

bool StencilState::enabled() const { return m_enabled; }

StencilFunction StencilState::func(StencilFace face) const
{
    Assert(
        face != StencilFace::FrontAndBack ||
            !(m_differentFrontAndBackBits & FUNC_BIT),
        "");
    return m_faces[faceToIndex(face)].func;
}

u32 StencilState::ref(StencilFace face) const
{
    Assert(
        face != StencilFace::FrontAndBack ||
            !(m_differentFrontAndBackBits & REF_BIT),
        "");
    return m_faces[faceToIndex(face)].ref;
}

u32 StencilState::readMask(StencilFace face) const
{
    Assert(
        face != StencilFace::FrontAndBack ||
            !(m_differentFrontAndBackBits & READMASK_BIT),
        "");
    return m_faces[faceToIndex(face)].readMask;
}

u32 StencilState::writeMask(StencilFace face) const
{
    Assert(
        face != StencilFace::FrontAndBack ||
            !(m_differentFrontAndBackBits & WRITEMASK_BIT),
        "");
    return m_faces[faceToIndex(face)].writeMask;
}

StencilOp StencilState::sfail(StencilFace face) const
{
    Assert(
        face != StencilFace::FrontAndBack ||
            !(m_differentFrontAndBackBits & SFAIL_BIT),
        "");
    return m_faces[faceToIndex(face)].sfail;
}

StencilOp StencilState::dpfail(StencilFace face) const
{
    Assert(
        face != StencilFace::FrontAndBack ||
            !(m_differentFrontAndBackBits & DPFAIL_BIT),
        "");
    return m_faces[faceToIndex(face)].dpfail;
}

StencilOp StencilState::dppass(StencilFace face) const
{
    Assert(
        face != StencilFace::FrontAndBack ||
            !(m_differentFrontAndBackBits & DPPASS_BIT),
        "");
    return m_faces[faceToIndex(face)].dppass;
}

void StencilState::setEnabled(bool enabled) { m_enabled = enabled; }

void StencilState::setFunc(StencilFunction func, StencilFace face)
{
    if (face == StencilFace::FrontAndBack)
    {
        m_faces[0].func = func;
        m_faces[1].func = func;
    }
    else
    {
        m_faces[faceToIndex(face)].func = func;
    }
    m_differentFrontAndBackBits = (m_faces[0].func != m_faces[1].func ? 1 : 0)
                                  << FUNC_SHIFT;
}

void StencilState::setRef(u32 ref, StencilFace face)
{
    if (face == StencilFace::FrontAndBack)
    {
        m_faces[0].ref = ref;
        m_faces[1].ref = ref;
    }
    else
    {
        m_faces[faceToIndex(face)].ref = ref;
    }
    m_differentFrontAndBackBits = (m_faces[0].ref != m_faces[1].ref ? 1 : 0)
                                  << REF_SHIFT;
}

void StencilState::setReadMask(u32 readMask, StencilFace face)
{
    if (face == StencilFace::FrontAndBack)
    {
        m_faces[0].readMask = readMask;
        m_faces[1].readMask = readMask;
    }
    else
    {
        m_faces[faceToIndex(face)].readMask = readMask;
    }
    m_differentFrontAndBackBits =
        (m_faces[0].readMask != m_faces[1].readMask ? 1 : 0) << READMASK_SHIFT;
}

void StencilState::setWriteMask(u32 writeMask, StencilFace face)
{
    if (face == StencilFace::FrontAndBack)
    {
        m_faces[0].writeMask = writeMask;
        m_faces[1].writeMask = writeMask;
    }
    else
    {
        m_faces[faceToIndex(face)].writeMask = writeMask;
    }
    m_differentFrontAndBackBits =
        (m_faces[0].writeMask != m_faces[1].writeMask ? 1 : 0)
        << WRITEMASK_SHIFT;
}

void StencilState::setSFail(StencilOp sfail, StencilFace face)
{
    if (face == StencilFace::FrontAndBack)
    {
        m_faces[0].sfail = sfail;
        m_faces[1].sfail = sfail;
    }
    else
    {
        m_faces[faceToIndex(face)].sfail = sfail;
    }
    m_differentFrontAndBackBits = (m_faces[0].sfail != m_faces[1].sfail ? 1 : 0)
                                  << SFAIL_SHIFT;
}

void StencilState::setDPFail(StencilOp dpfail, StencilFace face)
{
    if (face == StencilFace::FrontAndBack)
    {
        m_faces[0].dpfail = dpfail;
        m_faces[1].dpfail = dpfail;
    }
    else
    {
        m_faces[faceToIndex(face)].dpfail = dpfail;
    }
    m_differentFrontAndBackBits =
        (m_faces[0].dpfail != m_faces[1].dpfail ? 1 : 0) << DPFAIL_SHIFT;
}

void StencilState::setDPPass(StencilOp dppass, StencilFace face)
{
    if (face == StencilFace::FrontAndBack)
    {
        m_faces[0].dppass = dppass;
        m_faces[1].dppass = dppass;
    }
    else
    {
        m_faces[faceToIndex(face)].dppass = dppass;
    }
    m_differentFrontAndBackBits =
        (m_faces[0].dppass != m_faces[1].dppass ? 1 : 0) << DPPASS_SHIFT;
}

std::string StencilState::toString() const { return "Unimplemented"; }
}
