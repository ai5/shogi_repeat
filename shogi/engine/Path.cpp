#include "Path.h"
#include <stdlib.h>

#ifndef _WIN32
#include <limits.h>
#include <libgen.h>
#include <cstring>
#endif


/*-----------------------------------------------------------------------------*/
/**
* @brief Full Path 取得
* @param ファイル名
* @note
*/
/*-----------------------------------------------------------------------------*/
std::string Path::FullPath(const std::string& filename)
{
#ifdef _WIN32
	char path[_MAX_PATH] = { '\0' };

	_fullpath(path, filename.c_str(), _MAX_PATH);
#else
	char path[PATH_MAX] = { '\0' };
	realpath(filename.c_str(), path);
#endif
	
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
#ifdef _WIN32
	char path[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char file[_MAX_FNAME];
	char ext[_MAX_EXT];

#ifdef _MSC_VER
	_splitpath_s(filepath.c_str(), drive, dir, file, ext);
	_makepath_s(path, drive, dir, NULL, NULL);
#else
	_splitpath_s(filepath.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, file, _MAX_FNAME, ext, _MAX_EXT);
	_makepath_s(path, _MAX_PATH , drive, dir, NULL, NULL);

#endif
	int len = strlen(path);
	if (len != 0)
	{
		if (path[len - 1] == '\\' || path[len - 1] == '/')
		{
			path[len - 1] = '\0';
		}
	}

	return path;
#else
	char path[PATH_MAX];
	strncpy(path, filepath.c_str(), PATH_MAX - 1);
	
	return dirname(path);
#endif
}


/*-----------------------------------------------------------------------------*/
/**
* @brief ファイル名の取得
* @param ファイル名
* @note
*/
/*-----------------------------------------------------------------------------*/
std::string Path::FileName(const std::string& filepath)
{
#ifdef _WIN32
	char path[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char file[_MAX_FNAME];
	char ext[_MAX_EXT];

#ifdef _MSC_VER
	_splitpath_s(filepath.c_str(), drive, dir, file, ext);
	_makepath_s(path, NULL, NULL, file, ext);
#else
	_splitpath_s(filepath.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, file, _MAX_FNAME, ext, _MAX_EXT);
	_makepath_s(path, _MAX_PATH, NULL, NULL, file, ext);

#endif
	return path;
#else
	char path[PATH_MAX];
	strncpy(path, filepath.c_str(), PATH_MAX - 1);
	
	return basename(path);
#endif
}
