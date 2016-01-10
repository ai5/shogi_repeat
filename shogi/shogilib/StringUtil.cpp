
#include "StringUtil.h"

#include <Windows.h>

#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <memory>

#include <fstream>
#include <sstream>


/*-----------------------------------------------------------------------------*/
/**
 * @brief stringのFor /
 45
 ]:;lkj mat
 * @note  どこぞのコピペ
 */
/*-----------------------------------------------------------------------------*/
std::string StringUtil::Format(const char* fmt, ...) 
{
	va_list     arguments;
	std::string buffer = "";

	assert(fmt);

	va_start(arguments, fmt);

	{
		int length = _vscprintf(fmt, arguments);
		if (length >= 0)
		{

			std::unique_ptr<char[]> buf(new char[length + 1]);

			int result = vsprintf_s(buf.get(), length + 1, fmt, arguments);
			if (result >= 0)
			{
				buffer = buf.get();

			}
		}
	}

	va_end(arguments);

	return buffer;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief string からwstringへ 文字列変換
* @note sjisのみ対応
*/
/*-----------------------------------------------------------------------------*/
std::wstring StringUtil::ConvertWStringFromString(const std::string& str)
{
	if (str.empty()) return std::wstring();

	int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), (int)str.size(), NULL, 0);

	std::wstring wstr(size, 0);

	MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstr[0], size);

	return wstr;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief wstring からstringへ 文字列変換
* @note sjisのみ対応
*/
/*-----------------------------------------------------------------------------*/
std::string StringUtil::ConvertStringFromWString(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();

	int size = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);

	std::string str(size, 0);

	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &str[0], size, NULL, NULL);

	return str;
}

#if 0
/*-----------------------------------------------------------------------------*/
/**
 * @brief 数値のパース
 * @param str 文字列
 * @param [out] num  数値
 * @note  
 */
/*-----------------------------------------------------------------------------*/
template<typename T>
bool StringUtil::ParseNum(const T& str, OUT int& num)
{
	size_t index = 0;
	bool minus = false;
	bool ret = false;

	num = 0;

	if (str.length() >= 1)
	{
		if (str[0] == '-')
		{
			// マイナス
			minus = true;
			index++;
		}
	}

	for (; index < str.length(); index++)
	{
		wchar_t c = str[index];

		if (c >= '0' && c <= '9')
		{
			num = num * 10;
			num = num + (c - '0');
		}
		else if (c == 'K' || c == 'k')
		{
			num = num * 1000;
			break;
		}
		else if (c == 'M' || c == 'm')
		{
			num = num * 1000 * 1000;
			break;
		}
		else
		{
			break;
		}

		// 最低1桁以上あると成功
		ret = true;
	}

	if (minus)
	{
		num = -num;
	}

	return ret;
}
#endif

/*-----------------------------------------------------------------------------*/
/**
* @brief 時間のパース
* @param str 文字列
* @return 時間を秒に直したものを返す
* @note "01:15"なら 75
*       "01:02:03"なら 1*60*60+2*60+3を返す
*/
/*-----------------------------------------------------------------------------*/
int StringUtil::ParseTime(const std::wstring& str)
{
	int time = 0;
	int sec = 0;

	for (auto ch : str)
	{
		if (ch >= '0' && ch <= '9')
		{
			sec = sec * 10;
			sec += ch - '0';
		}
		else if (ch == ':')
		{
			time = time * 60 + sec * 60;
			sec = 0;
		}
		else
		{
			// 他の文字は無視
		}
	}

	return time + sec;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief テキストファイル読み込み 
 * @param 
 * @note  
 */
/*-----------------------------------------------------------------------------*/
std::string StringUtil::Load(std::string filename)
{
	std::ifstream file;

	file.open(filename.c_str());

	if (!file.is_open()) {
		return "";
	}

	std::string str;
	std::string ret = "";


	while (std::getline(file, str))
	{
		
		ret += str + '\n';
	}

	file.close();

	return ret;
}

