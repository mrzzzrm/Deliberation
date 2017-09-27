#pragma once

#include <Deliberation/ECS/AbstractActivity.h>
#include <Deliberation/ECS/Component.h>

namespace deliberation
{

class ActivityComponent:
    public Component<ActivityComponent>
{
DELIBERATION_COMPONENT_NAME("Activities");
public:
    void renderImGui() override;

private:
    friend class ActivityComponentPrototype;

private:
    struct ActivityEntry
    {
        ~ActivityEntry();

        std::shared_ptr<AbstractActivity> activity;
        std::string lid;
    };

private:
    std::vector<std::unique_ptr<ActivityEntry>> m_activities;
};

}