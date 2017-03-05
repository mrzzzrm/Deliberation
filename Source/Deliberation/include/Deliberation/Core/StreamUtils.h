#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Deliberation/Core/Math/AABB.h>
#include <Deliberation/Core/Math/Ray2D.h>
#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/Math/Sphere.h>

namespace deliberation
{

template<typename StreamType, typename T, glm::precision precision>
StreamType & operator<<(StreamType && os, const glm::tvec2<T, precision> & v);

template<typename StreamType, typename T, glm::precision precision>
StreamType & operator<<(StreamType && os, const glm::tvec3<T, precision> & v);

template<typename StreamType, typename T, glm::precision precision>
StreamType & operator<<(StreamType && os, const glm::tvec4<T, precision> & v);

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
StreamType & operator<<(StreamType && os, const Ray2D & sphere);

template<typename StreamType>
StreamType & operator<<(StreamType && os, const AABB & aabb);


//template<typename T>
//std::string operator+(const std::string & s, const T & value);
//
//template<typename T>
//std::string operator+(const T & value, const std::string & s);
//
//template<typename T>
//std::string operator+(const char * s, const T & value);
//
//template<typename T>
//std::string operator+(const T & value, const char * s);

}

#include <Deliberation/Core/StreamUtils.inl>

