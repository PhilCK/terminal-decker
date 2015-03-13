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
  int32_t    x;
  int32_t    y;
  int32_t    width;
  int32_t    height;
  int32_t    xOffset;
  int32_t    yOffset;
  int32_t    xAdvance;
};

struct FontDataInfo
{
  int32_t      maxWidth;
  int32_t      maxHeight;
  int32_t      baseline;
  int32_t      scaleWidth;
  int32_t      scaleHeight;
  int32_t      lineHeight;

  // TODO: Kill this!!! Yuk
  std::unordered_map<uint16_t, CharDataInfo> characters;
};

FontDataInfo ParseData(const std::string &filename);

} // namespace


#endif // include guard