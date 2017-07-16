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
        std::cout << "Updating Transform" << std::endl;
        if (!m_newJson.is_object()) return;

        auto & transform = transform3DComponent.value();
        transform.setPosition(m_newJson.value("Position", glm::vec3(0.0f))); std::cout << "New position " << transform.position().x << std::endl;
        transform.setOrientation(m_newJson.value("Orientation", glm::quat()));
        transform.setCenter(m_newJson.value("Center", glm::vec3(0.0f)));
        transform.setScale(m_newJson.value("Scale", 1.0f));
    }
};

}