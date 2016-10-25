#include <Deliberation/Core/Assert.h>

#include <string.h>

namespace deliberation
{

template<typename PtrType>
BlobValueBase<PtrType>::BlobValueBase(PtrType * ptr, Type type):
    m_ptr(ptr),
    m_type(type)
{

}

template<typename PtrType>
std::string BlobValueBase<PtrType>::toString() const
{
    return m_type.toString(m_ptr);
}

inline CBlobValue::CBlobValue(const void * ptr, Type type):
BlobValueBase(ptr, type)
{

}

inline BlobValue::BlobValue(void * ptr, Type type):
    BlobValueBase(ptr, type)
{

}

inline BlobValue & BlobValue::operator=(const BlobValue & rhs)
{
    Assert(m_type == rhs.m_type, "");

    memcpy(m_ptr, rhs.m_ptr, m_type.size());
    return *this;
}

inline BlobValue & BlobValue::operator=(const CBlobValue & rhs)
{
    Assert(m_type == rhs.m_type, "");

    memcpy(m_ptr, rhs.m_ptr, m_type.size());
    return *this;
}

}