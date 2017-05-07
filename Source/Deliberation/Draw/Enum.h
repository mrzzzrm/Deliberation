#pragma once

#include <glbinding/gl/enum.h>

namespace deliberation
{

// clang-format off
enum class TextureType: unsigned int
{
    Texture2d   = (unsigned int)gl::GL_TEXTURE_2D,
    CubeMap     = (unsigned int)gl::GL_TEXTURE_CUBE_MAP,
    None
};


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

enum class BufferMapping
{
    ReadOnly,
    WriteOnly,
    ReadWrite
};

enum class QueryType: unsigned int
{
    TimeElapsed                   = (unsigned int)gl::GL_TIME_ELAPSED,
    SamplesPassed                 = (unsigned int)gl::GL_SAMPLES_PASSED,
    AnySamplesPassed              = (unsigned int)gl::GL_ANY_SAMPLES_PASSED,
    PrimitivesGenerated           = (unsigned int)gl::GL_PRIMITIVES_GENERATED,
    TransformFeedbackPrimitives   = (unsigned int)gl::GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
};

enum class DrawPrimitive: unsigned int
{
    Points                  = (unsigned int)gl::GL_POINTS,
    LineStrip               = (unsigned int)gl::GL_LINE_STRIP,
    LineLoop                = (unsigned int)gl::GL_LINE_LOOP,
    Lines                   = (unsigned int)gl::GL_LINES,
    LineStripAdjacency      = (unsigned int)gl::GL_LINE_STRIP_ADJACENCY,
    LinesAdjacency          = (unsigned int)gl::GL_LINES_ADJACENCY,
    TriangleStrip           = (unsigned int)gl::GL_TRIANGLE_STRIP,
    TriangleFan             = (unsigned int)gl::GL_TRIANGLE_FAN,
    Triangles               = (unsigned int)gl::GL_TRIANGLES,
    TriangleStripAdjacency  = (unsigned int)gl::GL_TRIANGLE_STRIP_ADJACENCY,
    TrianglesAdjacency      = (unsigned int)gl::GL_TRIANGLES_ADJACENCY,
    Patches                 = (unsigned int)gl::GL_PATCHES
};

enum class TextureWrap: unsigned int
{
    ClampToEdge         = (unsigned int)gl::GL_CLAMP_TO_EDGE,
    MirroredRepeat      = (unsigned int)gl::GL_MIRRORED_REPEAT,
    Repeat              = (unsigned int)gl::GL_REPEAT,
    ClampToBorder       = (unsigned int)gl::GL_CLAMP_TO_BORDER
};

enum class TextureFilter: unsigned int
{
    Nearest                 = (unsigned int)gl::GL_NEAREST,
    Linear                  = (unsigned int)gl::GL_LINEAR,

    // Only for min filter
    NearestMipmapNearest    = (unsigned int)gl::GL_NEAREST_MIPMAP_NEAREST,
    LinearMipmapNearest     = (unsigned int)gl::GL_LINEAR_MIPMAP_NEAREST,
    NearestMipmapLinear     = (unsigned int)gl::GL_NEAREST_MIPMAP_LINEAR,
    LinearMipmapLinear      = (unsigned int)gl::GL_LINEAR_MIPMAP_LINEAR
};

enum class StencilFunction: unsigned int
{
    Never           = (unsigned int)gl::GL_NEVER,
    Less            = (unsigned int)gl::GL_LESS,
    LessOrEqual     = (unsigned int)gl::GL_LEQUAL,
    Greater         = (unsigned int)gl::GL_GREATER,
    GreaterOrEqual  = (unsigned int)gl::GL_GEQUAL,
    Equal           = (unsigned int)gl::GL_EQUAL,
    NotEqual        = (unsigned int)gl::GL_NOTEQUAL,
    Always          = (unsigned int)gl::GL_ALWAYS
};

enum class StencilOp: unsigned int
{
    Keep            = (unsigned int)gl::GL_KEEP,
    Zero            = (unsigned int)gl::GL_ZERO,
    Replace         = (unsigned int)gl::GL_REPLACE,
    Increment       = (unsigned int)gl::GL_INCR,
    IncrementWrap   = (unsigned int)gl::GL_INCR_WRAP,
    Decrement       = (unsigned int)gl::GL_DECR,
    DecrementWrap   = (unsigned int)gl::GL_DECR_WRAP,
    Invert          = (unsigned int)gl::GL_INVERT
};
// clang-format on

}