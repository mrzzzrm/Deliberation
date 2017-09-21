#include <Deliberation/ECS/Transform3DComponent.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/ImGui/ImGui.h>

namespace deliberation
{

void Transform3DComponent::setPosition(const glm::vec3 & position)
{
    if (m_mode == Mode::RigidBodyRef) m_body->setPosition(position);
    else m_transform.setPosition(position);
}

void Transform3DComponent::setOrientation(const glm::quat & orientation)
{
    if (m_mode == Mode::RigidBodyRef) m_body->setOrientation(orientation);
    else m_transform.setOrientation(orientation);
}

void Transform3DComponent::setScale(float scale)
{
    if (m_mode == Mode::RigidBodyRef) m_body->setScale(scale);
    else m_transform.setScale(scale);
}

void Transform3DComponent::setTransform(const Transform3D & transform)
{
    Assert(m_mode == Mode::OwnInstance);
    m_transform = transform;
}

void Transform3DComponent::useOwnInstance()
{
    Assert(m_mode == Mode::NotSetUp);
    m_mode = Mode::OwnInstance;
}

const Transform3D & Transform3DComponent::transform() const
{
    if (m_mode == Mode::RigidBodyRef) return m_body->transform();
    else return m_transform;
}

void Transform3DComponent::referenceBody(const std::shared_ptr<RigidBody> & body)
{
    Assert(m_mode == Mode::NotSetUp);

    m_body = body;
    m_body->setPosition(m_transform.position());
    m_body->setOrientation(m_transform.orientation());
    m_body->setScale(m_transform.scale());

    m_mode = Mode::RigidBodyRef;
}

void Transform3DComponent::renderImGui() { ImGuiProperty(transform()); }

}