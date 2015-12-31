#pragma once


#ifndef SHOGI_TOKENIZER_H_
#define SHOGI_TOKENIZER_H_

#include <string>
#include <vector>

struct Token
{
	int pos;
	int length;

	Token(int p, int l)
	{
		this->pos = p;
		this->length = l;
	}
};

class Tokenizer
{
private:
	std::string input_string_;
	std::vector<Token> tokens_;
	std::string delim_;

	std::vector<Token>::iterator current_;

public:
	Tokenizer();
	Tokenizer(const std::string& str, const std::string& delm);

	~Tokenizer();

	void Set(const std::string& str, const std::string& delm);
	int Count() const { return this->tokens_.size(); }

	std::string GetToken();
	std::string GetToken(const std::string& str);
	void Previous();

	std::string GetRest();

private:
	void Split();

	bool isDelimitter(char c) const {
		return this->delim_.find(c) != std::string::npos;
	}

};


#endif
