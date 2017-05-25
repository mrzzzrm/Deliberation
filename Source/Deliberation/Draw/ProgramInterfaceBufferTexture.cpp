#include <Deliberation/Draw/ProgramInterfaceBufferTexture.h>

namespace deliberation
{

std::string ProgramInterfaceBufferTexture::toString() const
{
    static const char * TYPES[] = {"Float", "UnsignedInt", "Int"};
    return "{Name: " + m_name + "; Type: " + TYPES[(int)m_type] + "; Location: " +
        std::to_string(m_location) + "}";
}

}