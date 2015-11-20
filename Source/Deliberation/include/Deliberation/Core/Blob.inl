namespace deliberation
{

template<typename T>
Blob::Blob(const std::vector<T> & value)
{
    class Impl:
        public detail::IBlobImpl
    {
    public:
        Impl(const std::vector<T> & value):
            m_value(value)
        {

        }

        virtual std::unique_ptr<IBlobImpl> clone() const override
        {
            return std::unique_ptr<IBlobImpl>(new Impl(m_value));
        }

        virtual const void * ptr() const override
        {
            return m_value.data();
        }

        virtual std::size_t size() const override
        {
            return m_value.size() * sizeof(T);
        }

    private:
        std::vector<T> m_value;
    };

    m_impl = std::unique_ptr<detail::IBlobImpl>(new Impl(value));
}

template<typename T>
Blob::Blob(std::vector<T> && value)
{
    class Impl:
        public detail::IBlobImpl
    {
    public:
        Impl(std::vector<T> && value):
            m_value(std::move(value))
        {

        }

        virtual std::unique_ptr<IBlobImpl> clone() const override
        {
            return std::unique_ptr<IBlobImpl>(new Impl(m_value));
        }

        virtual const void * ptr() const override
        {
            return m_value.data();
        }

        virtual std::size_t size() const override
        {
            return m_value.size() * sizeof(T);
        }

    private:
        std::vector<T> m_value;
    };

    m_impl = std::unique_ptr<detail::IBlobImpl>(new Impl(value));
}

}

