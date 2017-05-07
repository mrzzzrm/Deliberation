#pragma once

#include <string>

#include <Deliberation/Core/Types.h>

namespace deliberation
{
class BlobValue;

template<typename PtrType>
class BlobValueBase
{
  public:
    std::string toString() const;

  protected:
    BlobValueBase(PtrType * ptr, Type type);

  protected:
    PtrType * m_ptr;
    Type      m_type;
};

class CBlobValue final : public BlobValueBase<const void>
{
  public:
    inline CBlobValue(const void * ptr, Type type);

  private:
    friend class BlobValue;
};

class BlobValue final : public BlobValueBase<void>
{
  public:
    inline BlobValue(void * ptr, Type type);

    inline BlobValue & operator=(const BlobValue & rhs);
    inline BlobValue & operator=(const CBlobValue & rhs);
};
}

#include <Deliberation/Core/BlobValue.inl>