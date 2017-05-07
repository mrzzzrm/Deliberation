#include <Deliberation/Core/Assert.h>

#include <glm/glm.hpp>

namespace deliberation
{
template<typename T>
constexpr PixelFormatEnum PixelFormat::resolve()
{
    return PixelFormat_None;
}

template<>
constexpr PixelFormatEnum PixelFormat::resolve<glm::vec3>()
{
    return PixelFormat_RGB_32_F;
}
}