#include "Tokenizer.h"



Tokenizer::Tokenizer()
{
}

Tokenizer::Tokenizer(const std::string& str, const std::string& delim)
{
	this->input_string_ = str;
	this->delim_ = delim;
	this->Split();

	this->current_ = this->tokens_.begin();
}


Tokenizer::~Tokenizer()
{
}

/*-----------------------------------------------------------------------------*/
/**
@brief 文字列設定
@param str 文字列
@param delm 区切り文字
*/
/*-----------------------------------------------------------------------------*/
void Tokenizer::Set(const std::string& str, const std::string& delim)
{
	this->input_string_ = str;
	this->delim_ = delim;

	this->Split();

	this->current_ = this->tokens_.begin();
}

/*-----------------------------------------------------------------------------*/
/**
@brief トークン取得
*/
/*-----------------------------------------------------------------------------*/
std::string Tokenizer::GetToken()
{
	if (this->current_ == this->tokens_.end())
	{
		return "";
	}

	std::vector<Token>::iterator ite = this->current_++;

	return this->input_string_.substr(ite->pos, ite->length);
}

/*-----------------------------------------------------------------------------*/
/**
@brief 指定文字までのtoken取得
*/
/*-----------------------------------------------------------------------------*/
std::string Tokenizer::GetToken(const std::string& str)
{
	if (this->current_ == this->tokens_.end())
	{
		return "";
	}


	int start = this->current_->pos;

	std::vector<Token>::iterator ite = this->current_;
	while (ite != this->tokens_.end())
	{
		if (this->input_string_.compare(ite->pos, str.length(), str) == 0)
		{
			break;
		}

		ite++;
	}

	this->current_ = ite;
	int len = 0;

	if (ite == this->tokens_.end())
	{
		// 終端の場合
		len = this->input_string_.length() - start;
	}
	else
	{
		len = ite->pos - start;
	}
	
	return this->input_string_.substr(start, len);
}

/*-----------------------------------------------------------------------------*/
/**
@brief 戻す
*/
/*-----------------------------------------------------------------------------*/
void Tokenizer::Previous()
{
	if (this->current_ == this->tokens_.begin())
	{
		this->current_--;
	}
}

/*-----------------------------------------------------------------------------*/
/**
@brief 残りの部分を取得
*/
/*-----------------------------------------------------------------------------*/
std::string Tokenizer::GetRest()
{
	if (this->current_ == this->tokens_.end())
	{
		return "";
	}

	int len = this->input_string_.length() - this->current_->pos;

	return this->input_string_.substr(this->current_->pos, len);
}


/*-----------------------------------------------------------------------------*/
/**
@brief 文字列分割
*/
/*-----------------------------------------------------------------------------*/
void Tokenizer::Split()
{
	this->tokens_.clear();

	unsigned pos = 0;

	while (pos < this->input_string_.size())
	{
		// skip
		while (pos < this->input_string_.size() && this->isDelimitter(this->input_string_[pos]))
		{
			pos++;
		}

		// 次の区切り文字までカウント
		int start = pos;
		while (pos < this->input_string_.size() && !this->isDelimitter(this->input_string_[pos]))
		{
			pos++;
		}

		unsigned len = pos - start;
		if (len <= 0)
		{
			break;
		}

		// std::string str = this->input_string_.substr(start, pos - start);
		this->tokens_.emplace_back(start, len);
	}
}
