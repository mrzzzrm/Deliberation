#pragma once

#include <string>

#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

void init();
void shutdown();

const std::string & prefixPath();
void setPrefixPath(const std::string & prefixPath);

std::string dataPath(const std::string path);

void EnableGLErrorChecks();
void EnableGLErrorChecksAndLogging();
void DisableGLErrorChecks();

}

