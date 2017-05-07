namespace deliberation
{
namespace detail
{
    template<typename T>
    template<typename U>
    NamedDataMemberOf<T>::NamedDataMemberOf(const std::string & name, U T::*ptr)
        : m_name(name)
    {
        struct NamedDataMemberOfImpl : INamedDataMemberOfImpl<T>
        {
            NamedDataMemberOfImpl(U T::*ptr) : m_ptr(ptr) {}

            virtual Type & type() const override { return Type::resolve<U>(); }

            virtual unsigned int offset(const T & t) const override
            {
                return OffsetOf(t, m_ptr);
            }

            U T::*m_ptr;
        };

        m_impl = std::shared_ptr<INamedDataMemberOfImpl<T>>(
            new NamedDataMemberOfImpl(ptr));
    }

    template<typename T>
    const std::string & NamedDataMemberOf<T>::name() const
    {
        return m_name;
    }

    template<typename T>
    Type & NamedDataMemberOf<T>::type() const
    {
        return m_impl->type();
    }

    template<typename T>
    unsigned int NamedDataMemberOf<T>::offset(const T & dummy) const
    {
        return m_impl->offset(dummy);
    }
}
}
