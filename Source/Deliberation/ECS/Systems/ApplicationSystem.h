#pragma once

#include <Deliberation/ECS/System.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Platform/Application.h>

namespace deliberation
{
class Application;
class Input;

class ApplicationSystem : public System<ApplicationSystem>
{
public:
    ApplicationSystem(World & world, Application & application)
        : Base(world), m_application(application)
    {
    }

    Application & application() const { return m_application; }
    DrawContext & drawContext() const { return m_application.drawContext(); }
    Input &       input() const { return m_application.input(); }

private:
    Application & m_application;
};
}