#pragma once
#ifndef SHOGI_PATH_H_
#define SHOGI_PAT_H_

#include <string>

class Path
{
public:
	// path 
	static std::string FullPath(const std::string& filename);
	static std::string FolderPath(const std::string& filepath);

};

#endif
