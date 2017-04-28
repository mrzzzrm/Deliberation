#pragma once

#include <string>

#include <Deliberation/Core/Blob.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

namespace detail
{

struct AttributeBinding
{
    AttributeBinding(const std::string & name, Blob data):
        name(name),
        data(std::move(data))
    {}

    std::string name;
    Blob        data;
};

}

}