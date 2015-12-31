
#include <iostream>
#include <algorithm>
#include <iterator>

#include "Move.h"



/**
*初期化
*/
void Move::Init()
{
	this->to_ = SQ_NONE;
	this->from_ = SQ_NONE;
	this->move_type_ = NO_MOVE;
	this->piece_ = NO_PIECE;
	this->capture_piece_ = NO_PIECE;
	this->side_ = NO_COLOR;
}

/**
* 同じかどうか
*/
bool Move::Equals(const Move& move) const
{
	bool ret = false;

	if ((this->move_type_ & DROP_FLAG) && (move.move_type_ & DROP_FLAG))
	{
		if (this->to_ == move.to_ && this->piece_ == move.piece_)
		{
			ret = true;
		}
	}
	else if ((this->move_type_ & MOVE_FLAG) && (move.move_type_ & MOVE_FLAG))
	{
		if (this->from_ == move.from_
			&& this->to_ == move.to_
			&& (this->move_type_ & MOVE_MASK) == (move.move_type_ & MOVE_MASK))
		{
			ret = true;
		}
	}
	else
	{
		if (this->move_type_ == move.move_type_)
		{
			ret = true;
		}
	}

	return ret;
}

/*====================================================================================*/
/* MoveKif */

MoveKif::MoveKif()
{
	this->Init();
}

MoveKif::MoveKif(const Move& move) : Move(move)
{
	this->current_ = -1;
	this->number_ = 0;
	this->time = 0;
	this->totalTime = 0;
}

MoveKif::MoveKif(const MoveKif& move) : Move(move)
{
	this->comments_ = move.comments_;
	this->branches_ = move.branches_;

	this->current_ = move.current_;
	this->number_ = move.number_;

	this->time = move.time;
	this->totalTime = move.totalTime;
}

MoveKif::MoveKif(MoveType move_type) : Move(move_type)
{
	this->current_ = -1;
	this->number_ = 0;
	this->time = 0;
	this->totalTime = 0;
}



MoveKif::~MoveKif()
{
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief 初期化
 */
/*-----------------------------------------------------------------------------*/
void MoveKif::Init()
{
	this->Move::Init();

	this->branches_.clear();

	this->comments_.clear();
	this->current_ = -1;
	this->number_ = 0;

	this->totalTime = 0;
	this->time = 0;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ブランチ追加
* @param parent 親の指し手
* @note 要素だけ追加
*/
/*-----------------------------------------------------------------------------*/
Moves* MoveKif::MakeBranch(Moves* parent)
{
	this->branches_.emplace_back(new Moves());

	this->current_ = this->branches_.size() - 1; // カレント変えとく

	Moves* moves = this->branches_.back().get();
	moves->set_parent(parent);

	return moves;
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief ブランチ追加
 * @param parent 親の指し手
 * @param move 追加する指し手
 * @note  
 */
/*-----------------------------------------------------------------------------*/
void MoveKif::AddBranch(Moves* parent, const MoveKif& move)
{
	this->branches_.emplace_back(new Moves());

	Moves* moves = this->branches_.back().get();

	moves->set_parent(parent);
	moves->push_back(move);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ブランチ追加
* @param parent 親の指し手
* @param move 追加する指し手
* @note
*/
/*-----------------------------------------------------------------------------*/
void MoveKif::AddBranch(Moves* parent, const Moves& moves)
{
	this->branches_.emplace_back(new Moves(moves));
	
	Moves* m = this->branches_.back().get();
	m->set_parent(parent);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ブランチの順番入れ替え
* @param parent 親の指し手
* @param move 追加する指し手
* @note
*/
/*-----------------------------------------------------------------------------*/
void MoveKif::SwapBranch(int left, int right)
{

	swap(this->branches_[left], this->branches_[right]);
	if (this->current_ == left)
	{
		this->current_ = right;
	}
	else if (this->current_ == right)
	{
		this->current_ = left;
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ブランチ削除
* @param parent 親の指し手
* @param move 追加する指し手
* @note
*/
/*-----------------------------------------------------------------------------*/
void MoveKif::DeleteBranch(int no)
{

	this->branches_.erase(this->branches_.begin() + no);

	// カレント位置を調整

	if (this->current_ == no)
	{
		this->current_ = -1;
	}
	else if (this->current_ > no)
	{
		this->current_--;
	}

}

/*-----------------------------------------------------------------------------*/
/**
* @brief 親との入れ替え
* @param int no
* @param move 置き換える
* @note
*/
/*-----------------------------------------------------------------------------*/
void MoveKif::SwapParent(int no, std::shared_ptr<Moves> moves)
{
	this->branches_[no] = moves;

	if (this->current_ == -1)
	{
		this->current_ = no;
	}
	else if (this->current_ == no)
	{
		this->current_ = -1;
	}
}

/*====================================================================================*/
/* Moves */

Moves::Moves() : std::deque<MoveKif>()
{
	this->parent_ = nullptr;
}


/*-----------------------------------------------------------------------------*/
/**
* @brief コピーした後親を全部書き換える
* @note
*/
/*-----------------------------------------------------------------------------*/
Moves::Moves(const Moves& moves) : std::deque<MoveKif>(moves)
{
	Moves::ChangeParent(*this, moves.parent_);
}

/*-----------------------------------------------------------------------------*/
/**
* @brief コピーした後親を全部書き換える
* @note
*/
/*-----------------------------------------------------------------------------*/
Moves& Moves::operator=(const Moves& moves) {

	if (this != &moves)
	{
		this->deque<MoveKif>::operator=(moves);

		Moves::ChangeParent(*this, moves.parent_);
	}

	return *this;
}

/*-----------------------------------------------------------------------------*/
/**
* @brief コピーした後親を全部書き換える
* @note
*/
/*-----------------------------------------------------------------------------*/
void Moves::ChangeParent(Moves& moves, Moves* parent)
{
	moves.parent_ = parent;

	for (auto& move : moves)
	{
		for (auto& branch : move.branches())
		{
			Moves::ChangeParent(*branch, &moves);
		}
	}
}

/*-----------------------------------------------------------------------------*/
/**
* @brief ブランチ全削除
* @note
*/
/*-----------------------------------------------------------------------------*/
void MoveKif::ClearBranch()
{

	this->branches_.clear();
	this->current_ = -1;
}
