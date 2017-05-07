#pragma once

#include <Deliberation/Core/Math/AABB.h>

#include <Deliberation/Physics/CollisionObject.h>

namespace deliberation
{
class BroadphaseProxy final
{
  public:
    BroadphaseProxy(void * userData, const AABB & bounds = AABB());

    void *       userData() const;
    const AABB & bounds() const;
    size_t       index() const;

    void setBounds(const AABB & bounds);
    void setIndex(size_t index);

  private:
    void * m_userData;
    AABB   m_bounds;
    size_t m_index;
};
}