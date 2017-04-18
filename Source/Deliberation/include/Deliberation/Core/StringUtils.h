#pragma once

#include <string>
#include <vector>

namespace deliberation
{

bool StringStartsWith(const std::string & str, const std::string & testStr);
bool StringEndsWith(const std::string & str, const std::string & testStr);
bool StringContains(const std::string & str, const std::string & testStr);

std::string & StringRErase(std::string & str, std::size_t num);
std::string StringRErased(const std::string & str, std::size_t num);

std::string & StringRErase(std::string & str, const std::string & testStr);
std::string StringRErased(const std::string & str, const std::string & testStr);

std::string StringJoin(const std::vector<std::string> & strs, const std::string & sep);

}

