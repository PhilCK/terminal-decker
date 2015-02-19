#ifndef TEXT_DATA_PARSE_INCLUDED_B222201B_FD30_475A_AB9F_C2BD99CF946B
#define TEXT_DATA_PARSE_INCLUDED_B222201B_FD30_475A_AB9F_C2BD99CF946B

#include <string>

namespace FontData
{

struct CharDataInfo
{

};

struct FontDataInfo
{
  std::string fontName;
  uint32_t maxWidth;
  uint32_t maxHeight;



};

FontDataInfo ParseData(const std::string &filename)
{

  return FontDataInfo();
}

}


#endif // include guard