#ifndef TEXT_DATA_PARSE_INCLUDED_B222201B_FD30_475A_AB9F_C2BD99CF946B
#define TEXT_DATA_PARSE_INCLUDED_B222201B_FD30_475A_AB9F_C2BD99CF946B

#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <Caffeine/Common/Utilities/StringHelpers.hpp>
#include <Caffeine/Math/Math.hpp>
#include <unordered_map>
#include <string>
#include <fstream>

namespace FontData
{

struct CharDataInfo
{
  uint32_t    x;
  uint32_t    y;
  uint32_t    width;
  uint32_t    height;
  uint32_t    xOffset;
  uint32_t    yOffset;
  uint32_t    xAdvance;
};

struct FontDataInfo
{
  uint32_t      maxWidth;
  uint32_t      maxHeight;
  uint32_t      baseline;
  uint32_t      scaleWidth;
  uint32_t      scaleHeight;
  uint32_t      lineHeight;

  // TODO: Kill this!!! Yuk
  std::unordered_map<uint16_t, CharDataInfo> characters;
};

FontDataInfo ParseData(const std::string &filename);

} // namespace


#endif // include guard