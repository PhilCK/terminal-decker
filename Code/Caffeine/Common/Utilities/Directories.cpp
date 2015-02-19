
#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <Dirent/dirent.h>
#include <algorithm>


namespace Caffeine {
namespace Utilities {

std::string GetPathDir()
{
	static std::string path;

	char buffer[MAX_PATH];
	::GetModuleFileName(0, buffer, MAX_PATH);

	path = buffer;

	std::string::size_type lastDelim = path.find_last_of("\\/");
	if (lastDelim != std::wstring::npos)
		path.resize(lastDelim + 1);
	else
		path = "";
	
	return path;
}



void GetFileNamesOfTypeInDirectory(std::vector<std::string> & outVec, const std::string & ext, const std::string & directory)
{
	DIR *dir;
	struct dirent *ent;
	
	if((dir = opendir((directory).c_str())) != nullptr)
	{
		while((ent = readdir(dir)) != nullptr)
		{
			std::string file(ent->d_name);
				
			if(file.find("." + ext) != std::string::npos)
			{
				outVec.push_back(directory + "/" + file);
			}	
		}
		
		closedir(dir);
	}
	
	else
	{
		DEBUG_LOG("Failed opening dir.");
	}

}


std::string GetFileNameFromPath(const std::string &filename)
{
	auto PathSlashes = [](const char ch) -> bool
	{
		return ((ch == '\\') || (ch == '/'));
	};

	return std::string(std::find_if(filename.rbegin(), filename.rend(), PathSlashes).base(), filename.end());
}



} // namespace
} // namespace
