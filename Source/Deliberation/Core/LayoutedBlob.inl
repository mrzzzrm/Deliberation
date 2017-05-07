namespace deliberation
{
template<typename T>
TypedBlobValueAccessor<T> LayoutedBlob::field(const std::string & name)
{
    return field<T>(m_layout.field(name));
}

template<typename T>
CTypedBlobValueAccessor<T> LayoutedBlob::field(const std::string & name) const
{
    return field<T>(m_layout.field(name));
}

template<typename T>
TypedBlobValueAccessor<T> LayoutedBlob::field(const DataLayoutField & field)
{
    return TypedBlobValueAccessor<T>(m_data, m_layout, field);
}

template<typename T>
CTypedBlobValueAccessor<T>
LayoutedBlob::field(const DataLayoutField & field) const
{
    return CTypedBlobValueAccessor<T>(m_data, m_layout, field);
}

template<typename T>
TypedBlobIterator<T> LayoutedBlob::iterator(const std::string & name)
{
    return field<T>(name).iterator();
}

template<typename T>
TypedBlobIterator<T> LayoutedBlob::iterator(const DataLayoutField & field)
{
    return this->field<T>(field).iterator();
}

template<typename T>
CTypedBlobIterator<T> LayoutedBlob::citerator(const std::string & name) const
{
    return field<T>(name).citerator();
}

template<typename T>
CTypedBlobIterator<T>
LayoutedBlob::citerator(const DataLayoutField & field) const
{
    return this->field<T>(field).citerator();
}

template<typename T>
void LayoutedBlob::assign(
    const std::string & name, const std::vector<T> & values)
{
    field<T>(name).assign(values);
}
}