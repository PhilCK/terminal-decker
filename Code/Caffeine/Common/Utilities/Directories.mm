
#import <Caffeine/Utilities/Directories.hpp>
#import <Caffeine/Utilities/Logging.hpp>
#import <dirent.h>
#import <Foundation/Foundation.h>

namespace Caffeine {
namespace Utilities {

std::string GetPathDir()
{
	#ifndef DCAFF_UNIT_TEST
	static std::string path;

	if(path.empty())
	{
		NSBundle *bundle = [NSBundle mainBundle];
		
		if(bundle == nil) {
			return "";
		}
		else
		{
			NSString *rPath = [bundle resourcePath];
			path = [rPath UTF8String] + std::string("/");
		}
	}
	#else
	static std::string path = "";
	#endif
	
	return path;
}



void GetFileNamesOfTypeInDirectory(std::vector<std::string> &outVec, const std::string &ext, const std::string &directory)
{
	DIR *dir;
	struct dirent *ent;
	
	// Check dir has trailing slash.
	const std::string dirWithTrailingSlash = directory.substr(directory.length() - 1, 1) == "/" ? directory : directory + "/";
	
	if((dir = opendir(directory.c_str())) != nullptr)
	{
		while((ent = readdir(dir)) != nullptr)
		{
			if(ent->d_type == DT_REG)
			{
				std::string file(ent->d_name);
				
				if(file.find("." + ext) != std::string::npos)
				{
					outVec.push_back(dirWithTrailingSlash + file);
				}
			}
		}
		
		closedir(dir);
	}
	
	else
	{
		DEBUG_LOG("Failed opening dir.")
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
