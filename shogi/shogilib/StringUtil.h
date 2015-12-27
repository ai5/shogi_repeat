#pragma once
#ifndef SHOGI_STRING_UTIL_H_
#define shoGI_STRING_UTIL_H_

#include <string>



class StringUtil
{
public:
	static bool StartsWith(const std::string& str, const std::string& value) {
		return str.compare(0, str.length(), value) == 0;
	}

};


#endif

