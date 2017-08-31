#include <Deliberation/Core/FileModificationPoll.h>

#include <sys/types.h>
#include <unistd.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
FileModificationPoll::FileModificationPoll(const std::string & path)
    : m_path(path)
{
    m_mtime.tv_nsec = 0;
    m_mtime.tv_sec = 0;
}

const std::string & FileModificationPoll::path() const { return m_path; }

bool FileModificationPoll::check()
{
    struct stat s;

    auto r = stat(m_path.c_str(), &s);
    AssertM(r == 0, "Error stat'ing '" + m_path + "'");

    if (m_mtime.tv_nsec != s.st_mtim.tv_nsec ||
        m_mtime.tv_sec != s.st_mtim.tv_sec)
    {
        m_mtime = s.st_mtim;
        return true;
    }
    else
    {
        return false;
    }
}
}