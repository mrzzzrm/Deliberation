#pragma once

#include <memory>

/**
 * All top level objects in Draw/ (i.e. Buffer, Draw, Texture)... function as copy-able wrappers around an impl, in
 * order to avoid having to carry around std::shared_ptr<>s explicitly. Design decision back than...
 *
 * This base class provides a uniform interface for holding the impl and checking whether it is engaged.
 * @tparam ImplType
 */
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
