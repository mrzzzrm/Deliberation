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
    void         setJson(const Json & json) { m_json = json; }

    const Json & newJson() const { return m_newJson; }
    void         setNewJson(const Json & newJson)
    {
        if (m_newJson != newJson) m_jsonChangedFlag = true;
        m_newJson = newJson;
    }

    bool jsonChangedFlag() const { return m_jsonChangedFlag; }
    void setJsonChangedFlag(bool changed) { m_jsonChangedFlag = changed; }

    const std::string & name() const { return m_name; }
    void                setName(const std::string & name) { m_name = name; }

    void updateEntity(Entity entity)
    {
        if (!hasComponent(entity) || m_jsonChangedFlag) onUpdateEntity(entity);
    }

    virtual bool hasComponent(const Entity & entity) = 0;

protected:
    virtual void onUpdateEntity(Entity entity) = 0;

protected:
    Json        m_json;
    Json        m_newJson;
    std::string m_name;
    bool        m_jsonChangedFlag = true;
};
}