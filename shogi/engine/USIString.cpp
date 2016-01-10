#include <vector>

#include "USIString.h"




/*-----------------------------------------------------------------------------*/
/**
* @brief 数値のパース
* @param str 文字列
* @param [out] num  数値
* @note
*/
/*-----------------------------------------------------------------------------*/
bool USIString::ParseNum(const std::string& str, int* out_num)
{
	size_t index = 0;
	bool minus = false;
	bool ret = false;
	int num = 0;

	*out_num = 0;

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

	*out_num = num;

	return ret;
}

/*-----------------------------------------------------------------------------*/
/**
* token分割
*/
/*-----------------------------------------------------------------------------*/
std::string USITokenizer::GetToken()
{
	size_t pos = this->pos_;
	
	while (pos < this->input_string_.size() && this->input_string_[pos] == ' ')
	{
		pos++;
	}

	// 次の区切り文字までカウント
	int start = pos;
	while (pos < this->input_string_.size() && this->input_string_[pos] != ' ')
	{
		pos++;
	}

	this->pos_ = pos + 1;

	unsigned len = pos - start;
	if (len <= 0)
	{
		return "";
	}

	return this->input_string_.substr(start, len);
}

/*-----------------------------------------------------------------------------*/
/**
* token分割
*/
/*-----------------------------------------------------------------------------*/
std::string USITokenizer::GetToken(const std::string& next)
{
	size_t pos = this->pos_;

	// 空白スキップ
	// while (pos < this->input_string_.size() && this->input_string_[pos] == ' ')
	// {
	// 	pos++;
	// }

	// 次の区切り文字までカウント

	size_t start = pos;

	pos = this->input_string_.find(" " + next + " ", pos);
	if (pos == std::string::npos)
	{
		// ない場合
		pos = this->input_string_.size();
	}

	this->pos_ = pos + 1;

	unsigned len = pos - start;
	if (len <= 0)
	{
		return "";
	}

	return this->input_string_.substr(start, len);
}

/*-----------------------------------------------------------------------------*/
/**
* token分割
*/
/*-----------------------------------------------------------------------------*/
std::string USITokenizer::GetTokenName(const std::string& next)
{
	size_t pos = this->pos_;

	// 空白スキップ
	while (pos < this->input_string_.size() && this->input_string_[pos] == ' ')
	{
	 	pos++;
	}

	size_t start = pos;

	pos = this->input_string_.find(" " + next + " ", pos);
	if (pos == std::string::npos)
	{
		// ない場合
		pos = this->input_string_.size();
	}

	this->pos_ = pos + 1;

	unsigned len = pos - start;
	if (len <= 0)
	{
		return "";
	}

	return this->input_string_.substr(start, len);
}

/*-----------------------------------------------------------------------------*/
/**
* 最後まで
*/
/*-----------------------------------------------------------------------------*/
std::string USITokenizer::GetTokenLast()
{
	size_t pos = this->pos_;

	while (pos < this->input_string_.size() && this->input_string_[pos] == ' ')
	{
		pos++;
	}

	this->pos_ = this->input_string_.size();

	// 次の区切り文字までカウント
	unsigned len = this->input_string_.size() - pos;
	if (len <= 0)
	{
		return "";
	}


	return this->input_string_.substr(pos, len);
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 戻す
 */
/*-----------------------------------------------------------------------------*/
void USITokenizer::Back(const std::string& token)
{
	this->pos_ -= token.size(); // サイズ分戻すだけ
}