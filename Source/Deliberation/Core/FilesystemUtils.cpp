#include <Deliberation/Core/FilesystemUtils.h>

namespace deliberation
{

std::string GetDirFromPath(const std::string & path)
{
    if (path.back() == '/') return path; // Path is already a dir

    const auto lastSlashPos = path.rfind('/');

    if (lastSlashPos == std::string::npos) return "./"; // Path is just a file

    return path.substr(0, lastSlashPos + 1);
}

}