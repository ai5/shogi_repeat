#pragma once

#ifndef SHOGI_HISTORY_H_
#define SHOGI_HISTORY_H_

#include <deque>
#include <map>

#include "types.h"
#include "Key.h"

class Notation;
class Position;

class HistoryItem
{
public:
	int No;
	int Count;
	bool IsCheck;

	HistoryItem()
	{
		this->No = 0;
		this->Count = 0;
		this->IsCheck = false;
	}

	HistoryItem(int no, bool check)
	{
		this->No = no;
		this->Count = 1;
		this->IsCheck = check;
	}
};

class History
{
private:
	const int RepCount = 4;
	
	std::map<Key, HistoryItem> move_dic_;
	std::deque<Key>  move_list_;

public:
	History();
	~History();

	void Clear();
	void Init(const Notation& notation);
	void Add(const Key& hashkey, bool check);

	bool IsRepetition();
	bool IsRepetitionCheck();

};

#endif
