#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace deliberation
{

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::vec2 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::vec3 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::vec4 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::quat & q);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::mat3 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::mat4 & v);

}

#include <Deliberation/Core/StreamUtils.inl>

