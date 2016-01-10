#pragma once

#ifndef SHOGI_USI_STRING_H_
#define SHOGI_USI_STRING_H_

#include <string>
#include <vector>

class USIString
{
public:

	template <typename T1, typename T2>
	static bool StartsWith(const T1& str, const T2& value) {
		return str.compare(0, value.length(), value) == 0;
	}


	static bool ParseNum(const std::string& str, int* num);
};


class USITokenizer
{
	const std::string& input_string_;
	int pos_;

public:
	USITokenizer(const std::string& str) : input_string_(str)
	{
		this->pos_ = 0;
	}

	~USITokenizer() {};

	std::string GetToken();
	std::string GetToken(const std::string& next);
	std::string GetTokenLast();
	std::string GetTokenName(const std::string& next);

	void Back(const std::string& token);
};

#endif
