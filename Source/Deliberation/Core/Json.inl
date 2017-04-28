#define _USE_MATH_DEFINES
#include <math.h>

namespace deliberation
{
namespace detail
{

inline float JsonParseFloatString(const nlohmann::json & obj)
{
    if (obj == "PI") return M_PI;

    return obj.get<float>();
}

}
}

namespace nlohmann
{
    template <typename T, glm::precision P>
    void adl_serializer<glm::tvec3<T, P>>::to_json(json& j, const glm::tvec3<T, P> & v)
    {
        j = {v.x, v.y, v.z};
    }

    template <typename T, glm::precision P>
    void adl_serializer<glm::tvec3<T, P>>::from_json(const json& j, glm::tvec3<T, P> & v)
    {
        v.x = j[0];
        v.y = j[1];
        v.z = j[2];
    }

    template <typename T, glm::precision P>
    void adl_serializer<glm::tquat<T, P>>::to_json(json& j, const glm::tquat<T, P> & v)
    {
        j = {v.x, v.y, v.z, v.w};
    }

    template <typename T, glm::precision P>
    void adl_serializer<glm::tquat<T, P>>::from_json(const json& j, glm::tquat<T, P> & v)
    {
        if (j.size() == 3)
        {
            glm::vec3 euler;

            euler.x = deliberation::detail::JsonParseFloatString(j[0]);
            euler.y = deliberation::detail::JsonParseFloatString(j[1]);
            euler.z = deliberation::detail::JsonParseFloatString(j[2]);

            v = glm::quat(euler);
        }
        else
        {
            v.x = deliberation::detail::JsonParseFloatString(j[0]);
            v.y = deliberation::detail::JsonParseFloatString(j[1]);
            v.z = deliberation::detail::JsonParseFloatString(j[2]);
            v.w = deliberation::detail::JsonParseFloatString(j[3]);
        }
    }
}