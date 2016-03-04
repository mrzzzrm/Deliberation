#include "TextureLoaderDataImpl.h"

#include <iostream>
#include <vector>

#include <IL/il.h>
#include <IL/ilu.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/PixelFormat.h>

namespace deliberation
{

TextureLoaderDataImpl::TextureLoaderDataImpl(void * data, size_t length):
    m_data(data),
    m_length(length)
{

}

TextureBinary TextureLoaderDataImpl::load()
{
    auto pixels = std::vector<ILfloat>();

    auto width = ILint{0};
    auto height = ILint{0};

    {
        ILuint ilName;

        ilGenImages(1, &ilName);
        ilBindImage(ilName);

        auto r = ilLoadL(IL_PNG, m_data, m_length);
        if (r == IL_FALSE)
        {
            Fail(std::string("Failed to load texture: ") + iluErrorString(ilGetError()));
            return TextureBinary();
        }

        width = ilGetInteger(IL_IMAGE_WIDTH);
        height = ilGetInteger(IL_IMAGE_HEIGHT);

        pixels.resize(width * height * 3);

        ilCopyPixels(0u, 0u, 0u, width, height, 1, IL_RGB, IL_FLOAT, pixels.data());

        ilDeleteImage(ilName);
    }

    return TextureBinary(SurfaceBinary(std::move(pixels), width, height, PixelFormat_RGB_32_F));
}

}


