#pragma once

#include "Move.h"

#ifndef SHOGI_MOVES_ENUMERATOR_H_
#define SHOGI_MOVES_ENUMERATOR_H_

class MovesEnumerator
{

	const Moves& moves_;

	const Moves* current_moves_ = &moves_;
	int number_ = 0;

public:

	MovesEnumerator(const Moves& moves) : moves_(moves)
	{
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

	int no_ = -1;

public:

	MoveBranchEnumerator(const Moves* moves, const MoveKif* move)
		: moves_(moves), move_(move)
	{
	}

	~MoveBranchEnumerator()
	{
	}

	int current_no() const { return this->no_ - 1; }

	const MoveKif* Next();
};



#endif


