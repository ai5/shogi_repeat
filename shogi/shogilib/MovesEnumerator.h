#pragma once

#include "Move.h"

#ifndef SHOGI_MOVES_ENUMERATOR_H_
#define SHOGI_MOVES_ENUMERATOR_H_

class MovesEnumerator
{

	const Moves& moves_;

	const Moves* current_moves_;
	int number_;

public:

	MovesEnumerator(const Moves& moves) : moves_(moves)
	{
		this->current_moves_ = &this->moves_;
		this->number_ = 0;
	}

	~MovesEnumerator()
	{
	}

	const MoveKif& Current();

	void Reset();

	bool Next();
	bool IsEnd();
};


class MoveBranchEnumerator
{
	const Moves* moves_;
	const MoveKif* move_;

	int no_;

public:

	MoveBranchEnumerator(const Moves* moves, const MoveKif* move)
	{
		this->moves_ = moves;
		this->move_ = move;

		this->no_ = -1;
	}

	~MoveBranchEnumerator()
	{
	}

	int current_no() const { return this->no_ - 1; }

	const MoveKif* Next();
};



#endif


