#include "Path.h"
#include <stdlib.h>

/*-----------------------------------------------------------------------------*/
/**
* @brief Full Path 取得
* @param ファイル名
* @note
*/
/*-----------------------------------------------------------------------------*/
std::string Path::FullPath(const std::string& filename)
{
	char path[_MAX_PATH] = { '\0' };

	_fullpath(path, filename.c_str(), _MAX_PATH);

	return path;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief フォルダPath 取得
* @param ファイルパス
* @note
*/
/*-----------------------------------------------------------------------------*/
std::string Path::FolderPath(const std::string& filepath)
{
	char path[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char file[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath_s(filepath.c_str(), drive, dir, file, ext);

	_makepath_s(path, drive, dir, NULL, NULL);

	return path;
}


