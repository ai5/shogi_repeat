
#include "MovesEnumerator.h"



/*-----------------------------------------------------------------------------*/
/**
@brief リセット
*/
/*-----------------------------------------------------------------------------*/
void MovesEnumerator::Reset()
{
	this->number_ = 0;
	this->current_moves_ = &this->moves_;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief カレント取得
*/
/*-----------------------------------------------------------------------------*/
const MoveKif& MovesEnumerator::Current()
{
	int pos = this->number_ - (*this->current_moves_)[0].number();

	return (*this->current_moves_)[pos];
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 次の項目へ移動
*/
/*-----------------------------------------------------------------------------*/
bool MovesEnumerator::Next()
{
	int pos = this->number_ - (*this->current_moves_)[0].number();

	if (pos >= (int)this->current_moves_->size())
	{
		return false;
	}

	const MoveKif& move = (*this->current_moves_)[pos];

	if (move.getCurrentBranchNumber() >= 0)
	{
		// 分岐側
		this->current_moves_ = move.getCurrentBranch();
		pos = 0;
	}
	else
	{
		pos++;
	}

	this->number_++;

	if (pos >= (int)this->current_moves_->size())
	{
		// 終端の場合
		return false;
	}

	return true;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief 次の項目へ移動
*/
/*-----------------------------------------------------------------------------*/
bool MovesEnumerator::IsEnd()
{
	int pos = this->number_ - (*this->current_moves_)[0].number();

	if (pos >= (int)this->current_moves_->size())
	{
		// 終端の場合
		return true;
	}

	return false;
}




/*-----------------------------------------------------------------------------*/
/**
@brief ブランチのEnumerator
*/
/*-----------------------------------------------------------------------------*/
const MoveKif* MoveBranchEnumerator::Next()
{
	if (this->no_ == -1)
	{
		this->no_++;

		int pos = this->move_->number() - (*this->moves_)[0].number() + 1;
		
		if (pos < (int)this->moves_->size())
		{
			return &this->moves_->at(pos);
		}
	}

	if (this->no_ >= (int)this->move_->branches().size())
	{
		return nullptr;
	}

	return &this->move_->branches()[this->no_++]->at(0);
}
