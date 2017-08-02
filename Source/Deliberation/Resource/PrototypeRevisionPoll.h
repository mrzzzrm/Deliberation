#pragma once

#include <memory>

namespace deliberation
{

class AbstractPrototype;

class PrototypeRevisionPoll final
{
public:
    PrototypeRevisionPoll() = default;
    PrototypeRevisionPoll(const std::shared_ptr<AbstractPrototype> & prototype):
        m_prototype(prototype)
    {}

private:
    std::shared_ptr<AbstractPrototype> m_prototype;
};

}
