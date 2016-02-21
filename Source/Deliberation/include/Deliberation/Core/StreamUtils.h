#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Deliberation/Core/Math/Sphere.h>

namespace deliberation
{

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::uvec2 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::uvec3 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::uvec4 & v);

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

template<typename StreamType>
StreamType & operator<<(StreamType && os, const Sphere & sphere);

}

#include <Deliberation/Core/StreamUtils.inl>

