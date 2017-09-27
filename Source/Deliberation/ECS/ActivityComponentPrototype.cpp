#include <Deliberation/ECS/ActivityComponentPrototype.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Json.h>

namespace deliberation
{

void ActivityComponentPrototype::updateComponent(const Entity & entity, ActivityComponent & activityComponent)
{
    Assert(m_newJson.is_array());

    for (const auto & activityJson : m_newJson)
    {
        const auto name = activityJson["Activity"].get<std::string>();
        const auto lid = activityJson.value("LID", name);

        auto iter = std::find_if(activityComponent.m_activities.begin(),
                                 activityComponent.m_activities.end(),
                                 [&](const auto & rhs) {
            return rhs->lid == lid;
        });

        if (iter != activityComponent.m_activities.end())
        {
            activityComponent.m_activities.erase(iter);
        }

        auto activity = entity.world().activityManager()->createActivity(name);

        auto entry = std::make_unique<ActivityComponent::ActivityEntry>();
        entry->activity = activity;
        entry->lid = lid;

        activityComponent.m_activities.emplace_back(std::move(entry));

        activity->setEntity(entity);
        entity.world().activityManager()->addActivity(activity);
    }
}

}