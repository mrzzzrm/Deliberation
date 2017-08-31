#include <Deliberation/Core/StringUtils.h>

#include <numeric>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
bool StringStartsWith(const std::string & str, const std::string & testStr)
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

bool StringEndsWith(const std::string & str, const std::string & testStr)
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

bool StringContains(const std::string & str, const std::string & testStr)
{
    return str.find(testStr) != std::string::npos;
}

std::string & StringRErase(std::string & str, std::size_t num)
{
    Assert(str.size() >= num);
    str.resize(str.size() - num);
    return str;
}

std::string StringRErased(const std::string & str, std::size_t num)
{
    auto cpy = str;
    return StringRErase(cpy, num);
}

std::string & StringRErase(std::string & str, const std::string & testStr)
{
    AssertM(
        StringEndsWith(str, testStr),
        "'" + str + "' doesn't end with '" + testStr + "'");
    StringRErase(str, testStr.size());
    return str;
}

std::string StringRErased(const std::string & str, const std::string & testStr)
{
    auto cpy = str;
    return StringRErase(cpy, testStr);
}

std::string
StringJoin(const std::vector<std::string> & strs, const std::string & sep)
{
    if (strs.empty()) return {};

    const auto len =
        std::accumulate(
            strs.begin(),
            strs.end(),
            0u,
            [](size_t sum, const std::string & s) { return sum + s.size(); }) +
        sep.size() * (strs.size() - 1);

    std::string result;
    result.reserve(len);

    for (size_t i = 0; i < strs.size() - 1; i++)
    {
        result += strs[i] + sep;
    }

    result += strs.back();

    return result;
}

std::string & StringReplace(std::string & str, const std::string & original, const std::string & substitute)
{
    const auto first = str.find(original);
    if (first == std::string::npos) return str;

    return str.replace(first, original.size(), substitute);
}
}
