#pragma once

#include <stdlib.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class LayoutedBlob;
class LayoutedBlobElement;

template<typename T>
class DELIBERATION_API LayoutedBlobElementBase
{
public:
    using Base = LayoutedBlobElementBase<T>;

public:
    T & blob() const;
    size_t index() const;

protected:
    LayoutedBlobElementBase(T & blob, size_t index);

protected:
    T &     m_blob;
    size_t  m_index = 0;
};

class DELIBERATION_API CLayoutedBlobElement final:
    public LayoutedBlobElementBase<const LayoutedBlob>
{
public:
    CLayoutedBlobElement(const LayoutedBlob & blob, size_t index);

private:
    friend class LayoutedBlobElement;
};

class DELIBERATION_API LayoutedBlobElement final:
    public LayoutedBlobElementBase<LayoutedBlob>
{
public:
    LayoutedBlobElement(LayoutedBlob & blob, size_t index);

    LayoutedBlobElement & operator=(const LayoutedBlobElement & rhs);
    LayoutedBlobElement & operator=(const CLayoutedBlobElement & rhs);
};

}

#include <Deliberation/Core/LayoutedBlobElement.inl>