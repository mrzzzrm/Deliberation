#pragma once

namespace deliberation
{

template<typename T>
class Optional final
{
public:
    Optional();

    template<typename ... Args>
    Optional(Args && ... args);

    template<typename ... Args>
    void reset(Args && ... args);

    bool engaged() const;

    T & get();
    const T & get() const;

    T * ptr();
    const T * ptr() const;

    void disengage();

    T * operator->();
    const T * operator->() const;

    bool operator==(const T * other) const;
    bool operator!=(const T * other) const;

    bool operator==(const T & other) const;
    bool operator!=(const T & other) const;

private:
    template<typename ... Args>
    void reset(Args && ... args) const;

private:
    bool m_engaged;

    static_assert(sizeof(T) > 0, "Cannot take sizeof of undefined type");
    char m_data[sizeof(T)];
};

}

#include <Deliberation/Core/Optional.inl>
