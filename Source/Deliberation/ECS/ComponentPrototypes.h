#pragma once

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/ComponentPrototype.h>

namespace deliberation
{

class Transform3DComponentPrototype:
    public ComponentPrototype<Transform3DComponent>
{
public:
    void updateComponent(Transform3DComponent & transform3DComponent) override
    {
        if (m_json.is_object())
        {
            auto & transform = transform3DComponent.value();
            transform.setPosition(m_json.value("Position", glm::vec3(0.0f)));
            transform.setOrientation(m_json.value("Orientation", glm::quat()));
            transform.setCenter(m_json.value("Center", glm::vec3(0.0f)));
        }
    }
};

}