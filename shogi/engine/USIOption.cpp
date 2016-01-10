#include "USIOption.h"
#include "USIString.h"



USIOption::~USIOption()
{
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief オプション追加
 * @param tokens
 * @note  option name <id> type <t> default <x> [min <y> max <z>] {var <v>}
 */
/*-----------------------------------------------------------------------------*/
bool USIOptions::AddOption(const std::string &str)
{
	USITokenizer tok(str);

	if (tok.GetToken() != "option") return false;
	if (tok.GetToken() != "name") return false;

	std::string name = tok.GetTokenName("type");
	if (name == "") return false;

	if (tok.GetToken() != "type") return false;

	std::string type = tok.GetToken();
	if (type == "") return false;


	if (type == "button")
	{
		this->Add(new USIOptionButton(name));
	}
	else
	{
		if (tok.GetToken() != "default") return false;

		if (type == "check")
		{
			std::string value = tok.GetToken();
			if (value == "") return false;

			bool check = (value == "true") ? true : false;

			this->Add(new USIOptionCheck(name, check));
		}
		else if (type == "spin")
		{

			int value, min, max;

			if (!USIString::ParseNum(tok.GetToken(), &value))
			{
				return false;
			}

			if (tok.GetToken() != "min") return false;

			if (!USIString::ParseNum(tok.GetToken(), &min))
			{
				return false;
			}

			if (tok.GetToken() != "max") return false;

			if (!USIString::ParseNum(tok.GetToken(), &max))
			{
				return false;
			}

			this->Add(new USIOptionSpin(name, value, min, max));
		}
		else if (type == "combo")
		{
			std::string value = tok.GetToken("var");
			if (value == "") return false;

			std::vector<std::string> combo_values;

			while (1)
			{
				if (tok.GetToken() != "var")
				{
					break;
				}

				std::string var = tok.GetToken("var");
				if (var == "")
				{
					break;
				}

				combo_values.push_back(var);
			}

			this->Add(new USIOptionCombo(name, value, combo_values));
		}
		else if (type == "string")
		{
			this->Add(new USIOptionString(name, tok.GetTokenLast()));
		}
		else if (type == "filename")
		{
			this->Add(new USIOptionFilename(name, tok.GetTokenLast()));
		}
		else
		{
			return false;
		}
	}

	return true;
}