#pragma once

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/Entity.h>

namespace deliberation
{

class ComponentPrototypeBase
{
public:
    virtual ~ComponentPrototypeBase() = default;

    const Json & json() const { return m_json; }
    void setJson(const Json & json)
    {
        if (m_json != json) m_jsonChangedFlag = true;
        m_json = json;
    }

    bool jsonChangedFag() const { return m_jsonChangedFlag; }
    void setJsonChangedFlag(bool changed) { m_jsonChangedFlag = changed; }

    const std::string & name() const { return m_name; }
    void setName(const std::string & name) { m_name = name; }

    void updateEntity(Entity entity)
    {
        if (!hasComponent(entity) || m_jsonChangedFlag) onUpdateEntity(entity);
    }

    virtual bool hasComponent(const Entity & entity) = 0;

protected:
    virtual void onUpdateEntity(Entity entity) = 0;

protected:
    Json        m_json;
    std::string m_name;
    bool        m_jsonChangedFlag = true;
};

}