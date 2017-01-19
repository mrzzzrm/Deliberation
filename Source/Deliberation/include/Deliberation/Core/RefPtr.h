//#pragma once
//
//#include <memory>
//
//#include <Deliberation/Core/Assert.h>
//
//#include <Deliberation/Deliberation.h>
//
//namespace deliberation
//{
//
//template<typename T>
//class DELIBERATION_API RefPtr final
//{
//public:
//    RefPtr() = default;
//    RefPtr(const RefPtr & rhs);
//    ~RefPtr();
//
//    template<typename ... Args>
//    void reset(Args && ... args);
//
//    T * operator->();
//    const T * operator->() const;
//
//    T & operator*();
//    const T & operator*() const;
//
//private:
//    std::shared_ptr<T> m_value;
//};
//
//}
//
//#include <Deliberation/Core/RefPtr.inl>