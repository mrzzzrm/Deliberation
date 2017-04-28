#include <Deliberation/ECS/ComponentFilter.h>

namespace deliberation
{

ComponentFilter ComponentFilter::all()
{
    ComponentFilter filter;
    filter.m_components.set();

    return filter;
}

ComponentFilter ComponentFilter::none()
{
    ComponentFilter filter;
    return filter;
}

ComponentFilter::ComponentFilter()
{
}

bool ComponentFilter::accepts(const ComponentBitset & componentBits) const
{
    return (componentBits & m_components) == m_components;
}

}

