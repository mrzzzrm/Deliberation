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
        m_jsonChangedFlag = m_json != newJson;
        m_newJson = newJson;
    }

    bool jsonChangedFlag() const { return m_jsonChangedFlag; }
    void setJsonChangedFlag(bool changed) { m_jsonChangedFlag = changed; }

    const std::string & name() const { return m_name; }
    void                setName(const std::string & name) { m_name = name; }

    void updateEntity(Entity entity)
    {
        if (!hasComponent(entity)) std::cout << "ComponentPrototype: '" << name() << ": Adding to " << entity.name() << std::endl;
        else if (m_jsonChangedFlag) std::cout << "ComponentPrototype: '" << name() << ": JSON changed" << std::endl;

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