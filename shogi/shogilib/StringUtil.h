#pragma once
#ifndef SHOGI_STRING_UTIL_H_
#define shoGI_STRING_UTIL_H_

#include <string>


#define REF
#define OUT

template<typename T>
class StringUtilBase
{
public:
	static bool StartsWith(const T& str, const T& value) {
		return str.compare(0, value.length(), value) == 0;
	}

	static T TrimStart(const T& str, const T& trimchar)
	{
		int i;

		for (i = 0; i < (int)str.length(); i++)
		{
			if (trimchar.find(str[i]) == T::npos)
			{
				break;
			}
		}

		return str.substr(i);
	}

	//　前後をトリミング
	static T Trim(const T& str, const T& trimchar)
	{
		int start;

		for (start = 0; start < (int)str.length(); start++)
		{
			if (trimchar.find(str[start]) == T::npos)
			{
				break;
			}
		}

		int end;

		for (end = str.length() - 1; end >= 0; end--)
		{
			if (trimchar.find(str[end]) == T::npos)
			{
				break;
			}
		}


		return str.substr(start, end - start + 1);
	}

	static int IndexOfAny(const T& str, const T& any)
	{
		{
			int i;

			for (i = 0; i < (int)str.length(); i++)
			{
				if (any.find(str[i]) != T::npos)
				{
					// あった
					return i;
				}
			}

			return -1;
		}
	}
};


class StringUtil
{
public:
	static bool StartsWith(const std::string& str, const std::string& value) {
		return StringUtilBase<std::string>::StartsWith(str, value);
	}

	static bool StartsWith(const std::wstring& str, const std::wstring& value) {
		return StringUtilBase<std::wstring>::StartsWith(str, value);
	}

	static std::wstring TrimStart(const std::wstring& str, const std::wstring& value) {
		return StringUtilBase<std::wstring>::TrimStart(str, value);
	}

	static std::string TrimStart(const std::string& str, const std::string& value) {
		return StringUtilBase<std::string>::TrimStart(str, value);
	}

	static std::wstring Trim(const std::wstring& str, const std::wstring& value) {
		return StringUtilBase<std::wstring>::Trim(str, value);
	}

	static std::string Trim(const std::string& str, const std::string& value) {
		return StringUtilBase<std::string>::Trim(str, value);
	}

	static int IndexOfAny(const std::string& str, const std::string& any) {
		return StringUtilBase<std::string>::IndexOfAny(str, any);
	}

	static int IndexOfAny(const std::wstring& str, const std::wstring& any) {
		return StringUtilBase<std::wstring>::IndexOfAny(str, any);
	}

	static std::string Format(const char* fmt, ...);

	static std::wstring ConvertWStringFromString(const std::string& str);
	static std::string  ConvertStringFromWString(const std::wstring& wstr);


	static int ParseTime(const std::wstring& str);

	static std::string Load(std::string filename);

	/*-----------------------------------------------------------------------------*/
	/**
	* @brief 数値のパース
	* @param str 文字列
	* @param [out] num  数値
	* @note
	*/
	/*-----------------------------------------------------------------------------*/

	template<typename T1, typename T2>
	static bool ParseNum(const T1& str, OUT T2& num)
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
			else if (str[0] == '+')
			{
				// プラス
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
};


#endif

