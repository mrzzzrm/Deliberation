#pragma once

#include <Deliberation/Core/Detail/IBlobImpl.h>

namespace deliberation
{
namespace detail
{
    class ViewBlobImpl : public IBlobImpl
    {
      public:
        ViewBlobImpl(void * ptr, size_t size) : m_ptr(ptr), m_size(size) {}

        std::unique_ptr<IBlobImpl> clone() const override
        {
            return std::make_unique<ViewBlobImpl>(m_ptr, m_size);
        }
        virtual const void * ptr() const override { return m_ptr; }
        virtual std::size_t  size() const override { return m_size; }
        virtual void         resize(std::size_t size) override { Fail(""); }
        virtual void         reserve(std::size_t size) override { Fail(""); }

      private:
        void * m_ptr;
        size_t m_size;
    };
}
}
