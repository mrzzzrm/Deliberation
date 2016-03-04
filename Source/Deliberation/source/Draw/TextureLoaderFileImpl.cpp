#include "TextureLoaderFileImpl.h"

#include <cinttypes>
#include <iostream>
#include <vector>

#include <IL/il.h>
#include <IL/ilu.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/PixelFormat.h>

#include "ILUtils.h"

namespace deliberation
{

TextureLoaderFileImpl::TextureLoaderFileImpl(const std::string & path, Origin origin):
    m_path(path),
    m_origin(origin)
{

}

TextureBinary TextureLoaderFileImpl::load()
{
    auto pixels = std::vector<ILfloat>();

    auto width = ILint{0};
    auto height = ILint{0};
    auto format = PixelFormat();

    {
        ILuint ilName;

        ilGenImages(1, &ilName);
        ilBindImage(ilName);

        auto path = m_path;//TheResourcePathResolve::instance().resolve(m_path);

        auto r = ilLoadImage(path.c_str());
        if (r == IL_FALSE)
        {
            Fail(std::string("Failed to load texture '") + m_path + "': " + iluErrorString(ilGetError()));
            return TextureBinary();
        }

        // Set IL origin
        switch (m_origin)
        {
        case Origin::LowerLeft:
            ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
            break;
        case Origin::UpperLeft:
            ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
            break;
        default:
            Fail("");
        }

        width = ilGetInteger(IL_IMAGE_WIDTH);
        height = ilGetInteger(IL_IMAGE_HEIGHT);
        auto ilFormat = ilGetInteger(IL_IMAGE_FORMAT);
        format = ILFormatToPixelFormat(ilFormat);

        pixels.resize(width * height * format.componentsPerPixel());

        ilCopyPixels(0u, 0u, 0u, width, height, 1, ilFormat, IL_FLOAT, pixels.data());

        ilDeleteImage(ilName);
    }


    return TextureBinary(SurfaceBinary(std::move(pixels), width, height, format));
}

}

