#pragma once

#ifndef SHOGI_USI_OPTION_H_
#define SHOGI_USI_OPTION_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "USIString.h"

enum USIOptionType
{
	CHECK,
	SPIN,
	COMBO,
	BUTTON,
	STRING,
	FILENAME,
};

class USIOption
{
public:
	std::string Name;
	USIOptionType Type;

public:
	USIOption(const std::string& name, USIOptionType type)
	{
		this->Name = name;
		this->Type = type;
	}

	virtual ~USIOption();

	virtual std::string ValueToString() const { return ""; };

	virtual void SetValue(bool value) {}
	virtual void SetValue(int value) {}
	virtual void SetValue(const std::string& value) {}
	virtual void SetValue(const char* value) {}
};

// check
class USIOptionCheck : public USIOption
{
public:
	bool Value;
	bool DefaultValue;
public:
	USIOptionCheck(const std::string& name,  bool defaultValue) : USIOption(name, USIOptionType::CHECK)
	{
		this->DefaultValue = defaultValue;
		this->Value = defaultValue;
	}

	virtual std::string ValueToString() const { return (this->Value == false) ? "false" : "true"; }
	virtual void SetValue(bool value) { this->Value = value; }
	virtual void SetValue(const std::string& value) {
		this->Value = (value == "true") ? true : false;
	}

};


// スピン
class USIOptionSpin : public USIOption
{
public:
	int Value;
	int DefaultValue;
	int Min;
	int Max;

public:
	USIOptionSpin(const std::string& name,  int defaultValue, int min, int max) : USIOption(name, USIOptionType::SPIN)
	{
		this->Value = defaultValue;
		this->DefaultValue = defaultValue;
		this->Min = min;
		this->Max = max;
	}

	virtual std::string ValueToString() const { return std::to_string(this->Value); }
	virtual void SetValue(int value) { this->Value = value; }
	virtual void SetValue(const std::string& value) {
		USIString::ParseNum(value, &this->Value); 
	}
};

// コンボ
class USIOptionCombo : public USIOption
{
public:
	std::string Value;
	std::string DefaultValue;
	std::vector<std::string> ComboValues;

public:
	USIOptionCombo(const std::string& name, const std::string& defaultValue, const std::vector<std::string>& combo) : USIOption(name, USIOptionType::COMBO)
	{
		this->DefaultValue = defaultValue;
		this->Value = defaultValue;
		this->ComboValues = combo;
	}

	virtual std::string ValueToString() const { return this->Value; }
	virtual void SetValue(const std::string& value) { this->Value = value; }
	virtual void SetValue(const char* value) { this->Value = value; }
	virtual void SetValue(int value) {
		this->Value = ComboValues[value];
	}
};

class USIOptionButton : public USIOption
{
public:

public:
	USIOptionButton(const std::string& name) : USIOption(name, USIOptionType::BUTTON)
	{
	}
};

class USIOptionString : public USIOption
{
public:
	std::string Value;
	std::string DefaultValue;

public:
	USIOptionString(const std::string& name, const std::string& defaultValue) : USIOption(name, USIOptionType::STRING)
	{
		this->DefaultValue = defaultValue;
		this->Value = defaultValue;
	}

	virtual std::string ValueToString() const { return this->Value; }
	virtual void SetValue(const std::string& value) { this->Value = value; }
	virtual void SetValue(const char* value) { this->Value = value; }
};

class USIOptionFilename : public USIOption
{
public:
	std::string Value;
	std::string DefaultValue;


public:
	USIOptionFilename(const std::string& name, const std::string& defaultValue) : USIOption(name, USIOptionType::FILENAME)
	{
		this->DefaultValue = defaultValue;
		this->Value = defaultValue;
	}

	virtual std::string ValueToString() const { return this->Value; }
	virtual void SetValue(const std::string& value) { this->Value = value; }
	virtual void SetValue(const char* value) { this->Value = value; }

};

/*-----------------------------------------------------------------------------*/
/**
 * @brief オプションの連想配列
 */
/*-----------------------------------------------------------------------------*/
class USIOptions : public std::map<std::string, std::shared_ptr<USIOption>>
{
public:

	bool Contents(const std::string& name) const
	{
		return this->count(name) > 0;
	}

	void Clear()
	{
		this->clear();
	}

	USIOption* GetOption(const std::string& name)
	{
		if (this->Contents(name))
		{
			return (*this)[name].get();
		}

		return nullptr;
	}

	const USIOption* GetOption(const std::string& name) const 
	{
		if (this->Contents(name))
		{
			return (*this).at(name).get();
		}

		return nullptr;
	}


	void Add(USIOption* option)
	{
		//　登録されていても無視して追加
		(*this)[option->Name] = std::shared_ptr<USIOption>(option);
	}

	bool AddOption(const std::string& str);

};


#endif
