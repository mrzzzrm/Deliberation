#pragma once

#include <memory>

template<typename ImplType>
class AbstractDrawObject
{
public:
    using Base = AbstractDrawObject<ImplType>;

public:
    AbstractDrawObject() = default;
    explicit AbstractDrawObject(const std::shared_ptr<ImplType> & impl):
        m_impl(impl)
    {}
    virtual ~AbstractDrawObject() = default;

    bool isEngaged() const { return static_cast<bool>(m_impl); }

protected:
    std::shared_ptr<ImplType> m_impl;
};
