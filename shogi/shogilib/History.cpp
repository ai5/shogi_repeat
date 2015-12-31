#include "History.h"


#include "Notation.h"
#include "Position.h"
#include "MovesEnumerator.h"

#include "MoveCheck.h"

History::History()
{
}


History::~History()
{
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief ƒNƒŠƒA
 */
/*-----------------------------------------------------------------------------*/
void History::Clear()
{
	this->move_list_.clear();
	this->move_dic_.clear();
}

/*-----------------------------------------------------------------------------*/
/**
* @brief “r’†‚©‚çŽn‚ß‚éê‡‚Ì‰Šú‰»
*/
/*-----------------------------------------------------------------------------*/
void History::Init(const Notation& notation)
{
	Position pos = notation.initial_position();

	this->Clear();

	for (MovesEnumerator moves(notation.moves()); !moves.IsEnd(); moves.Next())
	{
		const MoveKif& move = moves.Current();

		if (is_move(move.move_type()))
		{
			pos.DoMove(move);

			if (move.move_type() != MoveType::PASS)
			{
				this->Add(pos.hashkey(), MoveCheck::IsCheck(pos));
			}
		}

		if (move.number() == notation.move_current().number())
		{
			break;
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief “o˜^
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void History::Add(const Key& key, bool check)
{
#if 0
	this->move_list_.push_back(key);

	if (this->move_dic_.count(key) > 0)
	{
		// Šù‚É‚ ‚éê‡
		this->move_dic_.at(key).Count += 1;
	}
	else
	{
		this->move_dic_[key] = HistoryItem(this->move_list_.size() - 1, check);	
	}
#endif
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief ç“úŽè”»’è
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool History::IsRepetition()
{
	if (this->move_list_.size() == 0)
	{
		return false;
	}

	auto key = this->move_list_.back();

	return this->move_dic_[key].Count >= RepCount;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief ˜A‘±‰¤Žè‚Ìç“úŽè”»’è
 * @note  
 */
/*-----------------------------------------------------------------------------*/
bool History::IsRepetitionCheck()
{
	bool ret = false;

	if (this->move_list_.size() == 0)
	{
		return false;
	}

	auto key = this->move_list_.back();

	if (this->move_dic_[key].Count >= RepCount && this->move_dic_[key].IsCheck)
	{
		ret = true;

		for (size_t i = this->move_dic_[key].No; i < this->move_list_.size(); i += 2)
		{
			key = this->move_list_[i];

			if (!this->move_dic_[key].IsCheck)
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
}

