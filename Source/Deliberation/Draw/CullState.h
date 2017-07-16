#pragma once

#include <string>
#include <type_traits>

#include <glbinding/gl/enum.h>

namespace deliberation
{
enum class CullFace : unsigned int
{
    Front = (unsigned int)gl::GL_FRONT,
    Back = (unsigned int)gl::GL_BACK,
    FrontAndBack = (unsigned int)gl::GL_FRONT_AND_BACK,

    None
};

enum class FrontFace : unsigned int
{
    CW = (unsigned int)gl::GL_CW,
    CCW = (unsigned int)gl::GL_CCW
};

class CullState final
{
public:
    static CullState disabled();

public:
    CullState(
        CullFace  cullFace = CullFace::Back,
        FrontFace frontFace = FrontFace::CCW);

    bool      enabled() const;
    CullFace  cullFace() const;
    FrontFace frontFace() const;

    void setEnabled(bool enabled);
    void setCullFace(CullFace cullFace);
    void setFrontFace(FrontFace frontFace);

    std::string toString() const;

private:
    bool      m_enabled;
    CullFace  m_cullFace;
    FrontFace m_frontFace;
};
}
