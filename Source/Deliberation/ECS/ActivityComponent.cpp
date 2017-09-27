#include <Deliberation/ECS/ActivityComponent.h>

#include <Deliberation/ECS/AbstractActivity.h>
#include <Deliberation/ECS/ActivityManager.h>

#include <Deliberation/ImGui/ImGui.h>

namespace deliberation
{

void ActivityComponent::renderImGui()
{
    for (const auto & activityEntry : m_activities)
    {
        ImGui::Text("%s", activityEntry->activity->name().c_str());
    }
}

ActivityComponent::ActivityEntry::~ActivityEntry()
{
    activity->manager()->removeActivity(activity);
}

}