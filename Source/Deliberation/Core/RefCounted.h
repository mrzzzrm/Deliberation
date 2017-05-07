//#pragma once
//
//#include <Deliberation/Core/Assert.h>
//
//#include <Deliberation/Deliberation.h>
//
// namespace deliberation
//{
//
// class RefCounted
//{
// public:
//    virtual ~RefCounted() = default;
//
//    u32 refCount() const { return m_refCount; }
//
//    void setRefCount(u32 refCount) { m_refCount = refCount; }
//
//    void incRefCount() { ++m_refCount; }
//
//    void decRefCount()
//    {
//        Assert(m_refCount > 0, "Can't dec ref count of 0");
//        m_refCount--;
//    }
//
// private:
//    u32 m_refCount;
//};
//
//}