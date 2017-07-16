#pragma once

#include <string>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <Deliberation/Draw/Enum.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
enum class StencilFace
{
    Front,
    Back,
    FrontAndBack
};

class StencilState final
{
public:
    struct Face
    {
        Face();
        Face(
            StencilFunction func,
            u32             ref,
            u32             readMask,
            u32             writeMask,
            StencilOp       sfail,
            StencilOp       dpfail,
            StencilOp       dppass);

        StencilFunction func = StencilFunction::Always;
        u32             ref = 0;
        u32             readMask = ~u32(0);
        u32             writeMask = ~u32(0);
        StencilOp       sfail = StencilOp::Keep;
        StencilOp       dpfail = StencilOp::Keep;
        StencilOp       dppass = StencilOp::Keep;
    };

public:
    StencilState();
    StencilState(const Face & frontAndBack);
    StencilState(
        StencilFunction func,
        u32             ref,
        u32             readMask,
        u32             writeMask,
        StencilOp       sfail,
        StencilOp       dpfail,
        StencilOp       dppass);
    StencilState(const Face & front, const Face & back);

    bool differentFaceFuncs() const;
    bool differentFaceMasks() const;
    bool differentFaceOps() const;

    const Face & frontFace() const;
    const Face & backFace() const;

    bool            enabled() const;
    StencilFunction func(StencilFace face = StencilFace::FrontAndBack) const;
    u32             ref(StencilFace face = StencilFace::FrontAndBack) const;
    u32       readMask(StencilFace face = StencilFace::FrontAndBack) const;
    u32       writeMask(StencilFace face = StencilFace::FrontAndBack) const;
    StencilOp sfail(StencilFace face = StencilFace::FrontAndBack) const;
    StencilOp dpfail(StencilFace face = StencilFace::FrontAndBack) const;
    StencilOp dppass(StencilFace face = StencilFace::FrontAndBack) const;

    void setEnabled(bool enabled);
    void
         setFunc(StencilFunction func, StencilFace face = StencilFace::FrontAndBack);
    void setRef(u32 ref, StencilFace face = StencilFace::FrontAndBack);
    void
    setReadMask(u32 readMask, StencilFace face = StencilFace::FrontAndBack);
    void
    setWriteMask(u32 writeMask, StencilFace face = StencilFace::FrontAndBack);
    void
    setSFail(StencilOp sfail, StencilFace face = StencilFace::FrontAndBack);
    void
    setDPFail(StencilOp dpfail, StencilFace face = StencilFace::FrontAndBack);
    void
    setDPPass(StencilOp dppass, StencilFace face = StencilFace::FrontAndBack);

    std::string toString() const;

private:
    bool     m_enabled;
    uint32_t m_differentFrontAndBackBits;
    Face     m_faces[2];
};
}
