#pragma once

#include <vector>

#include <Deliberation/Core/Detail/IBlobImpl.h>

namespace deliberation
{
namespace detail
{
    template<typename T>
    class VectorBlobImpl final : public IBlobImpl
    {
      public:
        VectorBlobImpl(std::size_t size);
        VectorBlobImpl(std::vector<T> && value);
        VectorBlobImpl(const std::vector<T> & value);

        virtual std::unique_ptr<IBlobImpl> clone() const override;
        virtual const void *               ptr() const override;
        virtual std::size_t                size() const override;
        virtual void                       resize(std::size_t size) override;
        virtual void                       reserve(std::size_t size) override;

      private:
        std::vector<T> m_value;
    };
}
}

#include <Deliberation/Core/Detail/VectorBlobImpl.inl>
