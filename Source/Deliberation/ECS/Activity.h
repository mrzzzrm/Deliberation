#pragma once

#include <string>
#include <memory>

#include <Deliberation/Core/TypeID.h>
#include <Deliberation/ECS/UntypedActivity.h>

namespace deliberation
{

template<typename T>
class Activity : public UntypedActivity
{
public:
    using Base = Activity<T>;

public:
    explicit Activity(const std::string & name = "Unnamed Activity");

    template<typename ... Args>
    void activatePhases();

    template<typename ... Args>
    void deactivatePhases();

    template<typename P>
    void activatePhase();

    template<typename P>
    void deactivatePhase();
};

}

#include <Deliberation/ECS/Activity.inl>