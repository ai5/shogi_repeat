#pragma once
#ifndef SHOGI_KEY_VALUE_H_
#define SHOGI_KEY_VALUE_H_

#include <string>
#include <map>

class KeyValue : public std::map<std::string, std::string>
{
public:
	bool Load(const std::string& filename);
	bool Save(const std::string& filename) const;
};


#endif

