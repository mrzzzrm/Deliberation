#pragma once

#include <sys/stat.h>

#include <string>

namespace deliberation
{

class FileModificationPoll final
{
public:
    FileModificationPoll() = default;
    FileModificationPoll(const std::string & path);

    const std::string & path() const;

    bool check();

private:
    std::string     m_path;
    struct timespec m_mtime;
};

}