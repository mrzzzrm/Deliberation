#include <Deliberation/Math/Morton.h>

#include <bitset>
#include <sstream>

#include <Deliberation/Core/StreamUtils.h>

namespace deliberation
{

std::string Morton3D::toString() const
{
    std::stringstream stream;
    stream << "(" << std::to_string(m_key) << " / " << std::bitset<32>(m_key).to_string() << " / " << toVec() << ")";
    return stream.str();
}

}

