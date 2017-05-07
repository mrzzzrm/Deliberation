#pragma once

#include <memory>
#include <vector>

#include <boost/optional.hpp>

#include <Deliberation/Core/LayoutedBlob.h>

namespace deliberation
{
class UniformImpl;
class DrawImpl;

class Uniform
{
  public:
    Uniform() = default;
    Uniform(const std::shared_ptr<DrawImpl> & drawImpl, size_t index);

    template<typename T>
    void set(const T & value);

    template<typename T>
    void set(const std::vector<T> & vec);

    void set(const LayoutedBlob & array);

    void set(const void * data, size_t size);

  private:
    std::shared_ptr<DrawImpl> m_drawImpl;
    const DataLayoutField *   m_field = nullptr; // Ref into m_drawImpl
    size_t                    m_index = 0;
};
}

#include <Deliberation/Draw/Uniform.inl>
