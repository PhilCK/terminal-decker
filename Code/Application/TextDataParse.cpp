
#include <Application/TextDataParse.hpp>

namespace FontData {

FontDataInfo ParseData(const std::string &filename)
{
  FontDataInfo font;

	std::ifstream fontDetails(CaffUtil::GetPathDir() + "Data/courier_new_font.txt");

  if(!fontDetails.is_open())
  {
    return font;
  }

	std::string line;

	font.maxWidth  = 0;
	font.maxHeight = 0;
	
	while(std::getline(fontDetails, line))
	{
		const std::vector<std::string> tokens = CaffUtil::SplitStringBySpaces(line);
			
		enum TokenID { CHAR = 0, ID, X, Y, WIDTH, HEIGHT, XOFFSET, YOFFSET, XADVANCE };
			
		if(!tokens.empty() && tokens.at(CHAR) == "char")
		{
			auto GetValue = [](const std::string &str) -> uint16_t
			{
				return CaffUtil::StringToInt(str.substr(str.find("=") + 1, str.length()));
			};
				
			CharDataInfo charInfo;
			charInfo.x			= GetValue(tokens.at(X));
			charInfo.y			= GetValue(tokens.at(Y));
			charInfo.width		= GetValue(tokens.at(WIDTH));
			charInfo.height		= GetValue(tokens.at(HEIGHT));
			charInfo.xOffset	= GetValue(tokens.at(XOFFSET));
			charInfo.yOffset	= GetValue(tokens.at(YOFFSET));
			charInfo.xAdvance	= GetValue(tokens.at(XADVANCE));
				
			font.maxWidth  = CaffMath::Max(font.maxWidth,  charInfo.width);
			font.maxHeight = CaffMath::Max(font.maxHeight, charInfo.height);
				
			font.characters[GetValue(tokens.at(ID))] = charInfo;
		}
	}
	
  return font;
}

} // namespace