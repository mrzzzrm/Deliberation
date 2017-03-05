#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <json.hpp>

namespace nlohmann
{
    template <typename T, glm::precision P>
    struct adl_serializer<glm::tvec3<T, P>>
    {
        static void to_json(json& j, const glm::tvec3<T, P> & v);
        static void from_json(const json& j, glm::tvec3<T, P> & v);
    };

    template <typename T, glm::precision P>
    struct adl_serializer<glm::tquat<T, P>>
    {
        static void to_json(json& j, const glm::tquat<T, P> & v);
        static void from_json(const json& j, glm::tquat<T, P> & v);
    };
}

#include <Deliberation/Core/Json.inl>