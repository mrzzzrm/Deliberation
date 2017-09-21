#pragma once

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/Transform3DComponent.h>
#include <Deliberation/ECS/ComponentPrototype.h>

namespace deliberation
{
class Transform3DComponentPrototype
    : public ComponentPrototype<Transform3DComponent>
{
public:
    void updateComponent(const Entity & entity, Transform3DComponent & transform3DComponent) override
    {
        if (!m_newJson.is_object()) return;

        transform3DComponent.setPosition(m_newJson.value("Position", glm::vec3(0.0f)));
        transform3DComponent.setOrientation(m_newJson.value("Orientation", glm::quat()));
        transform3DComponent.setScale(m_newJson.value("Scale", 1.0f));
    }
};
}