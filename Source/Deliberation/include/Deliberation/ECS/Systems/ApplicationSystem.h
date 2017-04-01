#pragma once

#include <Deliberation/ECS/System.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Platform/Application.h>

namespace deliberation
{

class Application;
class InputBase;

class ApplicationSystem:
    public System<ApplicationSystem>
{
public:
    ApplicationSystem(World & world, Application & application):
        Base(world),
        m_application(application)
    {}

    Application & application() const { return m_application; }
    Context & context() const { return m_application.context(); }
    InputBase & input() const  { return m_application.input(); }

private:
    Application & m_application;
};

}