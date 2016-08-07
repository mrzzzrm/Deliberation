#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Deliberation/Core/Math/AABB.h>
#include <Deliberation/Core/Math/Ray3D.h>
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
StreamType & operator<<(StreamType && os, const glm::ivec2 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::ivec3 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::ivec4 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::vec2 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::vec3 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::vec4 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::quat & q);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::mat2 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::mat3 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const glm::mat4 & v);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const Sphere & sphere);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const Ray3D & sphere);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const AABB & aabb);


template<typename T>
std::string operator+(const std::string & s, const T & value);

template<typename T>
std::string operator+(const T & value, const std::string & s);
//
//template<typename T>
//std::string operator+(const char * s, const T & value);
//
//template<typename T>
//std::string operator+(const T & value, const char * s);

}

#include <Deliberation/Core/StreamUtils.inl>

