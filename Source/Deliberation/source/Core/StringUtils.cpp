#include <Deliberation/Core/StringUtils.h>

namespace deliberation
{

bool StringStartsWith(const std::string & str, const std::string testStr)
{
    if (testStr.size() > str.size())
    {
        return false;
    }

    for (auto i = 0u; i < testStr.size(); i++)
    {
        if (str[i] != testStr[i])
        {
            return false;
        }
    }

    return true;
}

bool StringEndsWith(const std::string & str, const std::string testStr)
{
    if (testStr.size() > str.size())
    {
        return false;
    }

    auto startPos = str.size() - testStr.size();

    for (auto i = 0u; i < testStr.size(); i++)
    {
        if (str[i + startPos] != testStr[i])
        {
            return false;
        }
    }

    return true;
}

}

