#pragma once

#include <string>

#include <glbinding/gl/enum.h>

namespace deliberation
{

// TODO: Separate blend factors
// TODO: Constant Color/Alpha
// TODO: Blend state for individual draw buffers

enum class BlendEquation : unsigned int
{
    Add                 = (unsigned int)gl::GL_FUNC_ADD,
    Substract           = (unsigned int)gl::GL_FUNC_SUBTRACT,
    ReverseSubstract    = (unsigned int)gl::GL_FUNC_REVERSE_SUBTRACT,
    Min                 = (unsigned int)gl::GL_MIN,
    Max                 = (unsigned int)gl::GL_MAX,
    None
};

enum class BlendFactor
{
    Zero                    = (unsigned int)gl::GL_ZERO,
    One                     = (unsigned int)gl::GL_ONE,
    SourceColor             = (unsigned int)gl::GL_SRC_COLOR,
    OneMinusSourceColor     = (unsigned int)gl::GL_ONE_MINUS_SRC_COLOR,
    DestColor               = (unsigned int)gl::GL_DST_COLOR,
    OneMinusDestColor       = (unsigned int)gl::GL_ONE_MINUS_DST_COLOR,
    SourceAlpha             = (unsigned int)gl::GL_SRC_ALPHA,
    OneMinusSourceAlpha     = (unsigned int)gl::GL_ONE_MINUS_SRC_ALPHA,
    DestAlpha               = (unsigned int)gl::GL_DST_ALPHA,
    OneMinusDstAlpha        = (unsigned int)gl::GL_ONE_MINUS_DST_ALPHA,

    /* Not supported
    ConstantColor           = (unsigned int)gl::GL_CONSTANT_COLOR,
    OneMinusConstantColor   = (unsigned int)gl::GL_ONE_MINUS_CONSTANT_ALPHA,
    ConstantAlpha           = (unsigned int)gl::GL_CONSTANT_ALPHA,
    OneMinusConstantAlpha   = (unsigned int)gl::GL_ONE_MINUS_CONSTANT_ALPHA,
    */

    SourceAlphaSaturate     = (unsigned int)gl::GL_SRC_ALPHA_SATURATE,

    None

    // Dual source blending not supported, drivers don't as well, apparently
};

class BlendState final
{
public:
    static BlendState disabled();

public:
    BlendState();
    BlendState(BlendEquation equation, BlendFactor sfactor, BlendFactor dfactor);

    bool enabled() const;
    BlendEquation equation() const;
    BlendFactor sfactor() const;
    BlendFactor dfactor() const;

    std::string toString() const;

private:
    bool            m_enabled;
    BlendEquation   m_equation;
    BlendFactor     m_sfactor;
    BlendFactor     m_dfactor;
};

}


