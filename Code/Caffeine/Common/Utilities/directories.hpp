
#ifndef CAFFEINE_UTILITIES_DIRECTORIES_INCLUDED
#define CAFFEINE_UTILITIES_DIRECTORIES_INCLUDED

#include <Caffeine/Common/Platform.hpp>
#include <string>
#include <vector>

namespace Caffeine {
namespace Utilities {
  
std::string GetPathDir();

void GetFileNamesOfTypeInDirectory(std::vector<std::string> &outVec, const std::string &ext, const std::string &directory);

std::string GetFileNameFromPath(const std::string &filepath);

std::string GetCaffAssetDir();
std::string GetGameAssetDir();
  
} // namespace
} // namespace

#endif // include guard
