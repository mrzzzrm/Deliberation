#pragma once

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class BroadphaseProxy;

class BroadphaseFilter
{
  public:
    virtual ~BroadphaseFilter();

    virtual bool accepts(const BroadphaseProxy & proxy) const;
};

class BroadphaseBlacklistFilter : public BroadphaseBlacklistFilter
{
  public:
    BroadphaseBlacklistFilter(const std::shared_ptr<RigidBody> & body);

  private:
    std::shared_ptr<RigidBody> m_body;
};
}