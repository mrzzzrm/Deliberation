#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Deliberation/Core/Log.h>

template<typename T, glm::precision precision>
std::ostream & operator<<(std::ostream & os, const glm::tvec2<T, precision> & v);

template<typename T, glm::precision precision>
std::ostream & operator<<(std::ostream & os, const glm::tvec3<T, precision> & v);

template<typename T, glm::precision precision>
std::ostream & operator<<(std::ostream & os, const glm::tvec4<T, precision> & v);

std::ostream & operator<<(std::ostream & os, const glm::quat & q);
std::ostream & operator<<(std::ostream & os, const glm::mat2 & v);
std::ostream & operator<<(std::ostream & os, const glm::mat3 & v);
std::ostream & operator<<(std::ostream & os, const glm::mat4 & v);

// template<typename T>
// std::string operator+(const std::string & s, const T & value);
//
// template<typename T>
// std::string operator+(const T & value, const std::string & s);
//
// template<typename T>
// std::string operator+(const char * s, const T & value);
//
// template<typename T>
// std::string operator+(const T & value, const char * s);

namespace deliberation
{
template<typename T, glm::precision precision>
std::string ToString(const glm::tvec2<T, precision> & v);

template<typename T, glm::precision precision>
std::string ToString(const glm::tvec3<T, precision> & v);

template<typename T, glm::precision precision>
std::string ToString(const glm::tvec4<T, precision> & v);
}

#include <Deliberation/Core/StreamUtils.inl>
