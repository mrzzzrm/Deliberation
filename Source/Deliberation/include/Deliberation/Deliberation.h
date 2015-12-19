#pragma once

#include <string>

namespace deliberation
{

void init();
void shutdown();

const std::string & prefixPath();
void setPrefixPath(const std::string & prefixPath);

std::string dataPath(const std::string path);

void EnableGLErrorChecks();
void EnableGLErrorChecksAndLogging();

}

