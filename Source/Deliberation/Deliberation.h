#pragma once

#include <string>
#include <vector>

#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/Globals.h>

namespace deliberation
{
void init();
void shutdown();

const std::string & prefixPath();
void                setPrefixPath(const std::string & prefixPath);

std::string DeliberationDataPath(const std::string & path = ".");
std::vector<std::string>
DeliberationShaderPaths(const std::vector<std::string> & paths);

std::string GameDataPath(const std::string & path = ".");

void EnableGLErrorChecks();
void EnableGLErrorChecksAndLogging();
void DisableGLErrorChecks();

bool GLLoggingEnabled();
}
