#pragma once

#include <boost/optional.hpp>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/ECS/Component.h>

#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{

/**
 * Provides an Interface, whether this is provided by a RigidBody or by a held instance
 */
class Transform3DComponent
    : public Component<Transform3DComponent>
{
    DELIBERATION_COMPONENT_NAME("Transform3D")

public:
    enum class Mode {
        NotSetUp,
        OwnInstance,
        RigidBodyRef
    };

public:
    const Transform3D & transform() const;

    /**
     * Write access available for all modes
     */
    void setPosition(const glm::vec3 & position);
    void setOrientation(const glm::quat & orientation);
    void setScale(float scale);

    /**
     * Use only in Mode::OwnInstance
     */
    void setTransform(const Transform3D & transform);

    void useOwnInstance();
    void referenceBody(const std::shared_ptr<RigidBody> & body);

    void renderImGui() override;

private:
    Mode m_mode = Mode::NotSetUp;
    Transform3D m_transform;
    std::shared_ptr<RigidBody>   m_body;
};

}