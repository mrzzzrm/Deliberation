#pragma once

#include <Deliberation/Core/Json.h>

namespace deliberation
{

class ComponentPrototypeBase
{
public:
    virtual ~ComponentPrototypeBase() = default;

    const Json & json() const { return m_json; }
    void setJson(const Json & json) { m_json = json; }

    const std::string & name() const { return m_name; }
    void setName(const std::string & name) { m_name = name; }

    virtual void addComponentToEntity(Entity entity) = 0;
    virtual void updateEntity(Entity entity) = 0;

protected:
    Json        m_json;
    std::string m_name;
};

}