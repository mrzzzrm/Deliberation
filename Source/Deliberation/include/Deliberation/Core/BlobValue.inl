#include <Deliberation/Core/Assert.h>

namespace deliberation
{

inline CBlobValue::CBlobValue(const void * ptr, Type type):
    BlobValueBase{ptr, type}
{

}

inline BlobValue::BlobValue(void * ptr, Type type):
    BlobValueBase{ptr, type}
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