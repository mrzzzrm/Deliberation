#pragma once

#include <string>

#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/BlobValueAccessor.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlobElement.h>
#include <Deliberation/Core/TypedBlobValueAccessor.h>



namespace deliberation
{

class DataLayout;

class LayoutedBlob final
{
public:
    template<typename T> using Field = TypedBlobValueAccessor<T>;
    template<typename T> using CField = CTypedBlobValueAccessor<T>;
    template<typename T> using Iterator = TypedBlobIterator<T>;

public:
    LayoutedBlob();
    LayoutedBlob(const DataLayout & layout, size_t count = 0);
    LayoutedBlob(const DataLayout & layout, Blob && blob);

    const DataLayout & layout() const;
    bool empty() const;
    size_t count() const;
    const Blob & rawData() const;

    void write(std::size_t offset, const void * src, size_t length);

    template<typename T>
    TypedBlobValueAccessor<T> field(const std::string & name);

    template<typename T>
    CTypedBlobValueAccessor<T> field(const std::string & name) const;

    template<typename T>
    TypedBlobValueAccessor<T> field(const DataLayoutField & field);

    template<typename T>
    CTypedBlobValueAccessor<T> field(const DataLayoutField & field) const;
   
    BlobValueAccessor field(const std::string & name);
    CBlobValueAccessor field(const std::string & name) const;

    BlobValueAccessor field(const DataLayoutField & field);
    CBlobValueAccessor field(const DataLayoutField & field) const;

    template<typename T>
    TypedBlobIterator<T> iterator(const std::string & name);

    template<typename T>
    TypedBlobIterator<T> iterator(const DataLayoutField & field);

    template<typename T>
    CTypedBlobIterator<T> citerator(const std::string & name) const;

    template<typename T>
    CTypedBlobIterator<T> citerator(const DataLayoutField & field) const;

    template<typename T>
    void assign(const std::string & name, const std::vector<T> & values);

    void append(const CLayoutedBlobElement & element);
    void resize(size_t count);
    void reserve(size_t count);

    LayoutedBlobElement operator[](size_t index);
    CLayoutedBlobElement operator[](size_t index) const;

    std::string toString() const;

private:
    template<typename T> friend class LayoutedBlobElementBase;
    friend class CLayoutedBlobElement;
    friend class LayoutedBlobElement;

private:
    DataLayout m_layout;
    size_t     m_count = 0;
    Blob       m_data;
};

}

#include <Deliberation/Core/LayoutedBlob.inl>