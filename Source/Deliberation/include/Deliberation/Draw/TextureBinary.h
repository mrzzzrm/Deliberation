#pragma once

#include <array>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Draw/PixelFormat.h>
#include <Deliberation/Draw/SurfaceBinary.h>

namespace deliberation
{

class DELIBERATION_API TextureBinary final
{
public:
    static TextureBinary emptyTexture2D(unsigned int width,
                                        unsigned int height,
                                        PixelFormat format);

public:
    TextureBinary();
    TextureBinary(const SurfaceBinary & binary);
    TextureBinary(std::array<SurfaceBinary, 6> && cubeFaces);

    unsigned int width() const;
    unsigned int height() const;
    unsigned int numFaces() const;
    gl::GLenum type() const;
    PixelFormat format() const;
    const SurfaceBinary & surface(unsigned int face) const;

private:
    unsigned int                m_width;
    unsigned int                m_height;
    gl::GLenum                  m_type;
    PixelFormat                 m_format;
    std::vector<SurfaceBinary>  m_faces;
};

}

