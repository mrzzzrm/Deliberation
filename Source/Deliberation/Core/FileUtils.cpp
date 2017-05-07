#include <Deliberation/Core/FileUtils.h>

#include <fstream>
#include <streambuf>
#include <string>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
std::string FileToString(const std::string & path)
{
    std::ifstream file(path.c_str());
    Assert(file.is_open(), "Couldn't open '" + path + "'");

    file.seekg(0, std::ios::end);
    auto        size = file.tellg();
    std::string str(size, ' ');
    file.seekg(0, std::ios::beg);

    file.read(&str[0], size);

    file.close();

    return str;
}
}
