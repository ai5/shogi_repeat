#include "KeyValue.h"

#include <fstream>


#include "StringUtil.h"

/*-----------------------------------------------------------------------------*/
/**
 * @brief 読み込み
 * @note 日本語は無理
 */
/*-----------------------------------------------------------------------------*/
bool KeyValue::Load(const std::string& filename)
{
	this->clear();

	std::ifstream file;

	file.open(filename);

	if (!file.is_open()) {
		return false;
	}

	std::string str;

	while (std::getline(file, str))
	{
		if (str[0] == '#')
		{
			// コメント
		}
		else
		{
			int pos = StringUtil::IndexOfAny(str, "=\t");
			if (pos == std::string::npos)
			{
				// 無い場合 無視
			}
			else
			{
				// あった場合
				std::string key = str.substr(0, pos);
				std::string value = str.substr(pos + 1, str.length() - (pos + 1));

				key = StringUtil::Trim(key, " "); // 前後の空白はトリミング
				value = StringUtil::Trim(value, " ");

				(*this)[key] = value;
			}
		}
	}

	file.close();

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 保存
*/
/*-----------------------------------------------------------------------------*/
bool KeyValue::Save(const std::string& filename) const
{
	std::ofstream file;

	file.open(filename);

	if (!file.is_open()) {
		return false;
	}

	std::string str;

	for (auto& pair : *this)
	{
		file << pair.first << "=" << pair.second << std::endl;
	}

	file.close();

	return true;
}

