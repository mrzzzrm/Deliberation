#pragma once

#include <string>

#include <Deliberation/Core/IntTypes.h>



namespace deliberation
{

void init();
void shutdown();

const std::string & prefixPath();
void setPrefixPath(const std::string & prefixPath);

std::string DeliberationDataPath(const std::string path);
std::string GameDataPath(const std::string path);

void EnableGLErrorChecks();
void EnableGLErrorChecksAndLogging();
void DisableGLErrorChecks();

bool GLLoggingEnabled();

}

